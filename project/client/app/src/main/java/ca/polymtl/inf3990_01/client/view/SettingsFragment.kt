package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.support.v7.preference.PreferenceFragmentCompat
import ca.polymtl.inf3990_01.client.R

class SettingsFragment: PreferenceFragmentCompat() {
    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.preferences, rootKey)
    }
}