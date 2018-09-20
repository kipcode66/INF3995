package ca.polymtl.inf3995.team1.tp4;

/**
 * This class is equivalent to an enum and contains data about the different requests.
 */
public class RequestData {
    private static final RequestData REQUEST_DATA[] = new RequestData[3];

    public final int id;
    public final String name;
    public final String buttonText;
    public final String requestUrl;

    static {
        RequestData.REQUEST_DATA[0] = new RequestData(1, "test1", "Request test1", "http://132.207.89.11/test1");
        RequestData.REQUEST_DATA[1] = new RequestData(2, "test2", "Request test2", "http://132.207.89.11/test2");
        RequestData.REQUEST_DATA[2] = new RequestData(3, "test3", "Request test3", "http://132.207.89.11/test3");
    }

    public static final RequestData getInstance(int instanceId) {
        if (instanceId >= RequestData.REQUEST_DATA.length) {
            throw new RuntimeException("Invalide instance id: " + instanceId);
        }
        return RequestData.REQUEST_DATA[instanceId];
    }

    private RequestData(int id, String name, String buttonText, String requestUrl) {
        this.id = id;
        this.name = name;
        this.buttonText = buttonText;
        this.requestUrl = requestUrl;
    }
}
