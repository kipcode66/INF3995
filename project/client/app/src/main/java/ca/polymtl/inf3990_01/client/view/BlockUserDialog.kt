package ca.polymtl.inf3990_01.client.view

import android.app.Dialog
import android.content.Context
import android.view.animation.CycleInterpolator
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
            val areAllEmpty =
                block_username.text.trim().isEmpty() &&
                block_ip_address.text.trim().isEmpty() &&
                block_mac_address.text.trim().isEmpty()
            if (!areAllEmpty) {
                eventManager.dispatchEvent(UserBlockRequestEvent(
                    block_username.text.trim().toString(),
                    block_ip_address.text.trim().toString(),
                    block_mac_address.text.trim().toString()))
                this@BlockUserDialog.dismiss()
            }
            else if (block_username.text.trim().isEmpty()) {
                block_username.animate().translationX(8.0f).setInterpolator(CycleInterpolator(2.0f))
            }
            if (block_mac_address.text.trim().isEmpty()) {
                block_mac_address.animate().translationX(8.0f).setInterpolator(CycleInterpolator(2.0f))
            }
            if (block_ip_address.text.trim().isEmpty()) {
                block_ip_address.animate().translationX(8.0f).setInterpolator(CycleInterpolator(2.0f))
            }
        }
    }
}