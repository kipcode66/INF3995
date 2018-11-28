package ca.polymtl.inf3995.client.view

import android.os.Bundle
import android.os.Handler
import android.view.Menu
import android.view.MenuItem
import ca.polymtl.inf3995.client.R
import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.event.LogoutRequestEvent
import ca.polymtl.inf3995.client.controller.event.StatisticsRequestEvent
import ca.polymtl.inf3995.client.controller.state.AppStateService
import ca.polymtl.inf3995.client.model.DataProvider
import ca.polymtl.inf3995.client.presentation.StatisticsAdapter
import kotlinx.android.synthetic.main.content_statistics.*
import org.koin.android.ext.android.inject
import org.koin.core.parameter.ParameterList
import java.util.*

class StatisticsActivity: AbstractDrawerActivity(R.layout.activity_statistics, R.id.drawer_layout) {
    private val eventMgr: EventManager by inject()
    private val appStateService: AppStateService by inject()
    private val dataProvider: DataProvider by inject()
    private val statisticsAdapter: StatisticsAdapter by inject { ParameterList(dataProvider.getStatistics(), this.statisticsLayout) }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable?, arg: Any?) {
        Handler(this.mainLooper).post(this::invalidateOptionsMenu)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        appStateService.addObserver(Observer(this::onAppStateChange))
        this.statisticsLayout.setOnRefreshListener {
            eventMgr.dispatchEvent(StatisticsRequestEvent())
        }
        dataProvider.observeStatistics(Observer(this::onStatisticsUpdated))
        refresh()
    }

    private fun refresh() {
        Handler(mainLooper).post { statisticsLayout.isRefreshing = true }
        eventMgr.dispatchEvent(StatisticsRequestEvent())
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onStatisticsUpdated(o: Observable, arg: Any?) {
        statisticsAdapter.updateView()
        Handler(mainLooper).post { statisticsLayout.isRefreshing = false }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.statistics, menu)
        menu.findItem(R.id.action_show_login)
            .isVisible = appStateService.getState().type == AppStateService.State.User
        menu.findItem(R.id.action_disconnect)
            .isVisible = appStateService.getState().type == AppStateService.State.Admin
        menu.findItem(R.id.action_block_user)
            .isVisible = appStateService.getState().type == AppStateService.State.Admin
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_reload     -> {
                refresh()
                true
            }
            R.id.action_block_user -> {
                BlockUserDialog().show(supportFragmentManager, "block_user")
                return true
            }
            R.id.action_show_login -> {
                LoginDialog().show(supportFragmentManager, "login")
                return true
            }
            R.id.action_disconnect -> {
                eventMgr.dispatchEvent(LogoutRequestEvent())
                true
            }
            else                   -> super.onOptionsItemSelected(item)
        }
    }
}
