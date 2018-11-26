package ca.polymtl.inf3990_01.client.presentation

//import kotlinx.android.synthetic.main.statistics.view.*
import android.content.Context
import android.os.Handler
import android.view.View
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.Statistics
import ca.polymtl.inf3990_01.client.utils.Misc
import kotlinx.android.synthetic.main.content_statistics.view.*
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
        statistics = (arg as Statistics)
        updateView()
    }

    fun updateView() {
        val view = statisticsView
        val stat = dataProvider.getStatistics()
        view.songNumber.text = stat.chansons.toString()
        view.userNumber.text = stat.utilisateurs.toString()
        view.songDeletedNumber.text = stat.elemines.toString()
        view.duration.text = Misc.formatTime(stat.temps.toLong())
        Handler(appCtx.mainLooper).post(Runnable(statisticsView::invalidate))
    }

}