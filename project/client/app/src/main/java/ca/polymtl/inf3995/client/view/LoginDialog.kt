package ca.polymtl.inf3995.client.view

import android.app.AlertDialog
import android.app.Dialog
import android.os.Bundle
import android.support.v4.app.DialogFragment
import android.view.inputmethod.EditorInfo
import android.widget.Button
import ca.polymtl.inf3995.client.R
import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.event.LoginRequestEvent
import kotlinx.android.synthetic.main.login_layout.*
import org.koin.android.ext.android.inject

class LoginDialog: DialogFragment() {
    private val eventManager: EventManager by inject()
    private lateinit var submitButton: Button

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return activity?.let {
            // Use the Builder class for convenient dialog construction
            val builder = AlertDialog.Builder(it)
            builder.setTitle(R.string.login_dialog_title)
                .setPositiveButton(R.string.login_submit_text) { _, _ ->
                    eventManager
                        .dispatchEvent(LoginRequestEvent(dialog.login_username.text.toString(), dialog.login_password.text.toString()))
                }
                .setNegativeButton(android.R.string.cancel) { _, _ -> }
                .setView(R.layout.login_layout)

            // Create the AlertDialog object and return it
            builder.create()
        } ?: throw IllegalStateException("Activity cannot be null")
    }

    override fun onStart() {
        super.onStart()
        submitButton = (dialog as AlertDialog).getButton(AlertDialog.BUTTON_POSITIVE)
        dialog.login_password
            .setImeActionLabel(getText(R.string.login_submit_text), EditorInfo.IME_ACTION_DONE)
        dialog.login_password.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                submitButton.performClick()
            } else {
                false
            }
        }
    }
}