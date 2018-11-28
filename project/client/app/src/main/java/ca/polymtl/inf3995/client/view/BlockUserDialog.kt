package ca.polymtl.inf3995.client.view

import android.app.Dialog
import android.os.Bundle
import android.support.v4.app.DialogFragment
import android.support.v7.app.AlertDialog
import android.text.Editable
import android.text.TextWatcher
import android.view.View
import android.view.inputmethod.EditorInfo
import android.widget.Button
import ca.polymtl.inf3995.client.R
import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.event.UserBlockRequestEvent
import ca.polymtl.inf3995.client.utils.Misc
import kotlinx.android.synthetic.main.block_user_layout.*
import kotlinx.android.synthetic.main.block_user_layout.view.*
import org.koin.android.ext.android.inject

@Suppress("UsePropertyAccessSyntax")
class BlockUserDialog: DialogFragment() {
    private val eventManager: EventManager by inject()
    private lateinit var submitButton: Button

    private val textWatcher: TextWatcher = object: TextWatcher {
        override fun onTextChanged(s: CharSequence?, start: Int, before: Int, count: Int) {
            submitButton.isEnabled = !areAllFieldInvalid(dialog.block_user_dialog_layout)
        }

        override fun beforeTextChanged(s: CharSequence?, start: Int, count: Int, after: Int) {}

        override fun afterTextChanged(s: Editable?) {}
    }

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        return activity?.let {
            // Use the Builder class for convenient dialog construction
            val builder = AlertDialog.Builder(it)
            builder.setTitle(R.string.block_dialog_title)
                .setPositiveButton(R.string.block_submit_text) { _, _ ->
                    if (!areAllFieldInvalid(dialog.block_user_dialog_layout)) {
                        eventManager.dispatchEvent(UserBlockRequestEvent(
                            dialog.block_username.text.trim().toString(),
                            dialog.block_ip_address.text.trim().toString(),
                            dialog.block_mac_address.text.trim().toString().toUpperCase()))
                    }
                }
                .setNegativeButton(android.R.string.cancel) { _, _ -> }
                .setView(R.layout.block_user_layout)

            // Create the AlertDialog object and return it
            builder.create()
        } ?: throw IllegalStateException("Activity cannot be null")
    }

    override fun onStart() {
        super.onStart()
        submitButton = (dialog as AlertDialog).getButton(AlertDialog.BUTTON_POSITIVE)
        submitButton.isEnabled = !areAllFieldInvalid(dialog.block_user_dialog_layout)
        dialog.block_username.addTextChangedListener(textWatcher)
        dialog.block_ip_address.addTextChangedListener(textWatcher)
        dialog.block_mac_address.addTextChangedListener(textWatcher)
        dialog.block_mac_address.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                submitButton.performClick()
            } else {
                false
            }
        }
    }

    private fun areAllFieldInvalid(layout: View) =
        layout.block_username.text.trim().isEmpty() &&
        (layout.block_ip_address.text.trim().isEmpty() || !layout.block_ip_address.text.trim().matches(Misc.IP_REGEX)) &&
        (layout.block_mac_address.text.trim().isEmpty() || !layout.block_mac_address.text.trim().matches(Misc.MAC_REGEX))
}