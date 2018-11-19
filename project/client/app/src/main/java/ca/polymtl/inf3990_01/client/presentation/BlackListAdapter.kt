package ca.polymtl.inf3990_01.client.presentation

import android.content.Context
import android.os.Handler
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.UserList
import kotlinx.android.synthetic.main.black_list.view.*
import java.util.*


class BlackListAdapter(
        private val userList: UserList,
        private val layoutInflater: LayoutInflater,
        private val appCtx: Context,
        private val stateService: AppStateService,
        presenter: Presenter
): BaseAdapter() {
    init {
        presenter.addObserver(Observer(this::onPresenterUpdate))
        stateService.addObserver(Observer(this::onStateChange))
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onStateChange(o: Observable, arg: Any?) {
        Handler(appCtx.mainLooper).post(this::notifyDataSetChanged)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onPresenterUpdate(o: Observable, arg: Any?) {
        if (arg is UserList) {
            userList.clear()
            userList.addAll(arg)
            Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
        }
    }

    override fun getView(postion: Int, v: View?, viewGroup: ViewGroup?): View {
        val view = v ?: layoutInflater.inflate(R.layout.black_list, viewGroup, false)
        val user = this.userList[postion]
        view.name.text = user.name
        view.mac.text = user.mac
        view.ip.text = user.ip
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