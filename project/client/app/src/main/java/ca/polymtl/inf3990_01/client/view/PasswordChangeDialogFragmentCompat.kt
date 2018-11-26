package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.support.v7.app.AlertDialog
import android.support.v7.preference.PreferenceDialogFragmentCompat
import android.text.Editable
import android.text.TextWatcher
import android.view.View
import android.view.inputmethod.EditorInfo
import android.widget.Button
import android.widget.EditText
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.PasswordChangeRequestEvent
import org.koin.android.ext.android.inject


class PasswordChangeDialogFragmentCompat: PreferenceDialogFragmentCompat() {
    companion object {
        fun newInstance(key: String): PasswordChangeDialogFragmentCompat {
            val fragment = PasswordChangeDialogFragmentCompat()
            val b = Bundle(1)
            b.putString(PreferenceDialogFragmentCompat.ARG_KEY, key)
            fragment.arguments = b

            return fragment
        }
    }
    private lateinit var newPassword: EditText
    private lateinit var oldPassword: EditText
    private lateinit var submitButton: Button

    private val eventMgr: EventManager by inject()

    override fun onBindDialogView(view: View?) {
        super.onBindDialogView(view)
        view!!
        oldPassword = view.findViewById(R.id.old_password) as EditText
        newPassword = view.findViewById(R.id.new_password) as EditText
        oldPassword.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                newPassword.requestFocus()
            }
            else {
                false
            }
        }
        newPassword.setImeActionLabel(getString(R.string.change_password_button_submit), EditorInfo.IME_ACTION_DONE)
        newPassword.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE && !newPassword.text.trim().isEmpty()) {
                submitButton.performClick()
            }
            else {
                false
            }
        }
        newPassword.addTextChangedListener(object: TextWatcher {
            override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
                submitButton.isEnabled = !newPassword.text.trim().isEmpty()
            }

            override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) { }

            override fun afterTextChanged(s: Editable?) { }
        })
    }

    override fun onStart() {
        super.onStart()
        submitButton = (dialog as AlertDialog).getButton(AlertDialog.BUTTON_POSITIVE)
        submitButton.isEnabled = !newPassword.text.trim().isEmpty()
        submitButton.text = getString(R.string.change_password_button_submit)
    }

    override fun onDialogClosed(positiveResult: Boolean) {
        if (positiveResult) {
            val oldValue = oldPassword.text.trim().toString()
            val newValue = newPassword.text.trim().toString()
            if ((newValue as String?)?.isEmpty() == false) { // The "== false" is mandatory here, for the null check
                eventMgr.dispatchEvent(PasswordChangeRequestEvent(oldValue, newValue))
            }
        }
    }
}