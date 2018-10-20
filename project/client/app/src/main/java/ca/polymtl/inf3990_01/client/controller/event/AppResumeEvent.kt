package ca.polymtl.inf3990_01.client.controller.event

/**
 * An event that is fired when the app resumes from inactivity (when launching or coming back from being paused).
 */
class AppResumeEvent: Event {
    companion object {
        const val EVENT_TYPE = "app_resume"
    }
    override val type = EVENT_TYPE
}