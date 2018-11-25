package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.os.Handler
import android.view.Menu
import android.view.MenuItem
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.LogoutRequestEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import org.koin.android.ext.android.inject
import java.util.*

class SettingActivity : AbstractDrawerActivity(R.layout.activity_setting, R.id.drawer_layout) {
    private val eventMgr: EventManager by inject()
    private val appStateService: AppStateService by inject()

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable?, arg: Any?) {
        Handler(this.mainLooper).post(this::invalidateOptionsMenu)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        appStateService.addObserver(Observer(this::onAppStateChange))
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.setting, menu)
        menu.findItem(R.id.action_show_login).isVisible = appStateService.getState().type == AppStateService.State.User
        menu.findItem(R.id.action_disconnect).isVisible = appStateService.getState().type == AppStateService.State.Admin
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        return when (item.itemId) {
            R.id.action_show_login -> {
                val loginDialog = LoginDialog(this, eventMgr)
                loginDialog.show()
                true
            }
            R.id.action_disconnect -> {
                eventMgr.dispatchEvent(LogoutRequestEvent())
                true
            }
            else -> super.onOptionsItemSelected(item)
        }
    }
}
