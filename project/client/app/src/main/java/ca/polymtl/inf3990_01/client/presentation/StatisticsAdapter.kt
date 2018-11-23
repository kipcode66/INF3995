package ca.polymtl.inf3990_01.client.presentation

import android.content.Context
import android.os.Handler
import android.view.LayoutInflater
import android.view.View
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.Statistics
import kotlinx.android.synthetic.main.content_statistics.view.*
//import kotlinx.android.synthetic.main.statistics.view.*
import java.util.*


class StatisticsAdapter(
        private var statistics: Statistics,
        private val statisticsView: View,
        private val appCtx: Context,
        private val dataProvider: DataProvider
) {
    init {
        dataProvider.observeStatistics(Observer(this::onStatisticsUpdate))
        updateView()
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onStatisticsUpdate(o: Observable, arg: Any?) {
        statistics =  (arg as Statistics)
        updateView()
    }

    fun updateView() {
        val view = statisticsView
        val stat = this.statistics
        view.songNumber.text = statistics.chansons.toString()
        view.userNumber.text = statistics.utilisateurs.toString()
        view.songDeletedNumber.text = statistics.elemines.toString()
        view.duration.text = statistics.temps.toString()
        Handler(appCtx.mainLooper).post(Runnable(statisticsView::invalidate))
    }

}