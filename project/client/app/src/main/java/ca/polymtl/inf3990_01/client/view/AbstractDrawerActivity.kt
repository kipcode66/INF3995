package ca.polymtl.inf3990_01.client.view

import android.content.Intent
import android.os.Bundle
import android.support.annotation.IdRes
import android.support.annotation.LayoutRes
import android.support.design.widget.NavigationView
import android.support.v4.view.GravityCompat
import android.support.v4.widget.DrawerLayout
import android.support.v7.app.ActionBarDrawerToggle
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.Toolbar
import android.view.MenuItem
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.model.state.AppStateService
import org.koin.android.ext.android.inject
import java.util.Observer

abstract class AbstractDrawerActivity(
        @LayoutRes private val layoutRes: Int,
        @IdRes private val drawerLayoutId: Int
): AppCompatActivity(), NavigationView.OnNavigationItemSelectedListener {

    protected val stateService: AppStateService by inject()

    protected lateinit var drawerLayout: DrawerLayout
    protected lateinit var toolbar: Toolbar
    protected lateinit var navView: NavigationView

    private val stateObserver = Observer { o, arg ->
        if (o is AppStateService) {
            setNavItemsVisibility()
        }
    }

    protected fun setNavItemsVisibility() {
        navView.menu.findItem(R.id.nav_blacklist).isVisible = stateService.getState().canDisplayBlackList()
        navView.menu.findItem(R.id.nav_statistics).isVisible = stateService.getState().canDisplayStatistics()
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(layoutRes)
        drawerLayout = findViewById(drawerLayoutId)
        toolbar = drawerLayout.findViewWithTag(getString(R.string.tag_toolbar))
        navView = drawerLayout.findViewWithTag(getString(R.string.tag_nav_view))

        setSupportActionBar(toolbar)

        val toggle = ActionBarDrawerToggle(
                this, drawerLayout, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close)
        drawerLayout.addDrawerListener(toggle)
        toggle.syncState()

        setNavItemsVisibility()
        stateService.addObserver(stateObserver)
        navView.setNavigationItemSelectedListener(this)
    }

    override fun onDestroy() {
        stateService.deleteObserver(stateObserver)
        super.onDestroy()
    }

    override fun onBackPressed() {
        if (drawerLayout.isDrawerOpen(GravityCompat.START)) {
            drawerLayout.closeDrawer(GravityCompat.START)
        } else {
            super.onBackPressed()
        }
    }

    override fun onNavigationItemSelected(item: MenuItem): Boolean {
        // Handle navigation view item clicks here.
        when (item.itemId) {
            R.id.nav_queue -> {
                val intent = Intent(this, QueueActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
                startActivity(intent)
            }
            R.id.nav_local_song -> {
                val intent = Intent(this, LocalSongActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
                startActivity(intent)
            }
            R.id.nav_statistics -> {
                val intent = Intent(this, StatisticsActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
                startActivity(intent)
            }
            R.id.nav_blacklist -> {
                val intent = Intent(this, BlackListActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
                startActivity(intent)
            }
            R.id.nav_settings -> {
                val intent = Intent(this, SettingActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
                startActivity(intent)
            }
        }

        drawerLayout.closeDrawer(GravityCompat.START)
        return false
    }
}