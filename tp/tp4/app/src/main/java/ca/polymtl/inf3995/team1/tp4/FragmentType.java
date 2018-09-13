package ca.polymtl.inf3995.team1.tp4;

public class FragmentType {
    private static final FragmentType FRAGMENT_TYPES[] = new FragmentType[3];

    public final int id;
    public final String name;
    public final String buttonText;
    public final String requestUrl;

    static {
        FragmentType.FRAGMENT_TYPES[0] = new FragmentType(1, "test1", "Request test1", "http://132.207.89.11/test1");
        FragmentType.FRAGMENT_TYPES[1] = new FragmentType(2, "test2", "Request test2", "http://132.207.89.11/test2");
        FragmentType.FRAGMENT_TYPES[2] = new FragmentType(3, "test3", "Request test3", "http://132.207.89.11/test3");
    }

    public static final FragmentType getInstance(int instanceId) {
        if (instanceId >= FragmentType.FRAGMENT_TYPES.length) {
            throw new RuntimeException("Invalide instance id: " + instanceId);
        }
        return FragmentType.FRAGMENT_TYPES[instanceId];
    }

    public FragmentType(int id, String name, String buttonText, String requestUrl) {
        this.id = id;
        this.name = name;
        this.buttonText = buttonText;
        this.requestUrl = requestUrl;
    }
}
