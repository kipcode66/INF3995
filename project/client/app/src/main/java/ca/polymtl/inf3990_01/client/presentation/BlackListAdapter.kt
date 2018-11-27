package ca.polymtl.inf3990_01.client.presentation

import android.content.Context
import android.os.Handler
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.UserUnblockRequestEvent
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.UserList
import kotlinx.android.synthetic.main.black_list.view.*
import java.util.*


class BlackListAdapter(
        private val userList: UserList,
        private val layoutInflater: LayoutInflater,
        private val appCtx: Context,
        private val dataProvider: DataProvider,
        private val eventMgr: EventManager
): BaseAdapter() {
    init {
        dataProvider.observeBlackList(Observer(this::onBlackListUpdate))
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onBlackListUpdate(o: Observable, arg: Any?) {
        userList.clear()
        userList.addAll(arg as UserList)
        Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
    }

    override fun getView(postion: Int, v: View?, viewGroup: ViewGroup?): View {
        val view = v ?: layoutInflater.inflate(R.layout.black_list, viewGroup, false)
        val user = this.userList[postion]
        view.name.text = if (!user.name.isEmpty()) user.name else appCtx.getString(R.string.no_name)
        view.name.isSelected = true
        view.mac.text = user.mac
        view.mac.isSelected = true
        view.ip.text = user.ip
        view.ip.isSelected = true
        view.unblock.setOnClickListener {
            eventMgr.dispatchEvent(UserUnblockRequestEvent(user.name, user.ip, user.mac))
        }
        return view
    }

    override fun getItemId(p: Int): Long {
        return  p.toLong()
    }

    override fun getCount(): Int {
        return this.userList.size
    }

    override fun getItem(item: Int): Any {
        return this.userList[item]
    }
}
