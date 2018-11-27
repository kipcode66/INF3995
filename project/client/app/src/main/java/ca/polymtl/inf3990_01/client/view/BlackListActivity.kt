package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.os.Handler
import android.view.Menu
import android.view.MenuItem
import android.widget.ListView
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.LogoutRequestEvent
import ca.polymtl.inf3990_01.client.controller.event.RequestBlackListReloadEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.UserList
import ca.polymtl.inf3990_01.client.presentation.BlackListAdapter
import kotlinx.android.synthetic.main.content_black_list.*
import org.koin.android.ext.android.inject
import org.koin.core.parameter.ParameterList
import java.util.*

class BlackListActivity : AbstractDrawerActivity(R.layout.activity_black_list, R.id.drawer_layout) {

    private val usersList : UserList = UserList()
    private val eventMgr: EventManager by inject()
    private val appStateService: AppStateService by inject()
    private val dataProvider: DataProvider by inject()
    private val blackListAdapter: BlackListAdapter by inject{ ParameterList(usersList, layoutInflater) }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable?, arg: Any?) {
        Handler(this.mainLooper).post(this::invalidateOptionsMenu)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        appStateService.addObserver(Observer(this::onAppStateChange))
        val usersListView = this.findViewById(R.id.users_list) as ListView
        usersListView.adapter = blackListAdapter
        content_queue_swipe.setColorSchemeResources(R.color.colorPrimary, R.color.colorPrimaryDark)
        content_queue_swipe.setOnRefreshListener { eventMgr.dispatchEvent(RequestBlackListReloadEvent()) }
        dataProvider.observeBlackList(Observer(this::onBlackListChange))
        refresh()
    }

    private fun refresh() {
        Handler(mainLooper).post {content_queue_swipe.isRefreshing = true}
        eventMgr.dispatchEvent(RequestBlackListReloadEvent())
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onBlackListChange(o: Observable, arg: Any?) {
        Handler(mainLooper).post {content_queue_swipe.isRefreshing = false}
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.black_list, menu)
        menu.findItem(R.id.action_show_login).isVisible = appStateService.getState().type == AppStateService.State.User
        menu.findItem(R.id.action_disconnect).isVisible = appStateService.getState().type == AppStateService.State.Admin
        menu.findItem(R.id.action_block_user).isVisible = appStateService.getState().type == AppStateService.State.Admin
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        when (item.itemId) {
            R.id.action_reload -> {
                refresh()
                return true
            }
            R.id.action_block_user -> {
                BlockUserDialog(this, eventMgr).show()
                return true
            }
            R.id.action_show_login -> {
                val loginDialog = LoginDialog(this, eventMgr)
                loginDialog.show()
                return true
            }
            R.id.action_disconnect -> {
                eventMgr.dispatchEvent(LogoutRequestEvent())
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }
}
