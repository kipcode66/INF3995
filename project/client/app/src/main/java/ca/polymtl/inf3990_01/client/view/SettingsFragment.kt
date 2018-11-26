package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.support.v4.app.DialogFragment
import android.support.v7.preference.Preference
import android.support.v7.preference.PreferenceFragmentCompat
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import org.koin.android.ext.android.inject
import java.util.*

class SettingsFragment: PreferenceFragmentCompat() {
    private val appStateService: AppStateService by inject()
    private lateinit var adminPassword: Preference

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.preferences, rootKey)
        adminPassword = findPreference(getString(R.string.settings_key_admin_password))
        adminPassword.isVisible = appStateService.getState().type == AppStateService.State.Admin
        appStateService.addObserver(this::onAppStateChange)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable, arg: Any?) {
        adminPassword.isVisible = appStateService.getState().type == AppStateService.State.Admin
    }

    override fun onDisplayPreferenceDialog(preference: Preference?) {
        var dialogFragment: DialogFragment? = null
        if (preference is PasswordChangeDialogPreference) {
            dialogFragment = PasswordChangeDialogFragmentCompat
                .newInstance(preference.key)
        }

        if (dialogFragment != null) {
            dialogFragment.setTargetFragment(this, 0)
            dialogFragment.show(this.fragmentManager,
                "android.support.v7.preference" +
                ".PreferenceFragment.DIALOG")
        }
        else {
            super.onDisplayPreferenceDialog(preference)
        }
    }

}