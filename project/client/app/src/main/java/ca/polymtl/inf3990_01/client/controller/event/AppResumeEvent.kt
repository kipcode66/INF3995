package ca.polymtl.inf3990_01.client.controller.event

class AppResumeEvent: Event {
    companion object {
        const val EVENT_TYPE = "app_resume"
    }
    override val type = EVENT_TYPE
}