package ca.polymtl.inf3990_01.client.view

import android.content.Context
import android.content.res.TypedArray
import android.support.v7.preference.DialogPreference
import android.util.AttributeSet
import ca.polymtl.inf3990_01.client.R

class PasswordChangeDialogPreference(
    activityCtx: Context,
    attrs: AttributeSet? = null
): DialogPreference(activityCtx, attrs) {
    init {
        isPersistent = false
        dialogLayoutResource = R.layout.password_change
    }

    override fun onGetDefaultValue(a: TypedArray?, index: Int): Any {
        return ""
    }

    override fun onSetInitialValue(defaultValue: Any?) { }
}