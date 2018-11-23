package ca.polymtl.inf3990_01.client.view

import android.app.Dialog
import android.content.Context
import android.view.inputmethod.EditorInfo
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.UserBlockRequestEvent
import kotlinx.android.synthetic.main.block_user_layout.*

class BlockUserDialog(
    activityCtx: Context,
    private val eventManager: EventManager
): Dialog(activityCtx, R.style.Dialog) {
    init {
        setTitle(R.string.block_dialog_title)
        setCancelable(true)
        setCanceledOnTouchOutside(true)
        setOnCancelListener { this@BlockUserDialog.dismiss() }
        setContentView(R.layout.block_user_layout)
        block_mac_address.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE) {
                block_button_submit.performClick()
            }
            else {
                false
            }
        }
        block_button_submit.setOnClickListener {
            eventManager.dispatchEvent(UserBlockRequestEvent(block_username.text.toString(), block_ip_address.text.toString(), block_mac_address.text.toString()))
            this@BlockUserDialog.dismiss()
        }
    }
}