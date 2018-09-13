package ca.polymtl.inf3995.team1.tp4;

public class FragmentType {
    private static final FragmentType FRAGMENT_TYPES[] = new FragmentType[3];

    public final int id;
    public final String name;

    static {
        FragmentType.FRAGMENT_TYPES[0] = new FragmentType(1, "test1");
        FragmentType.FRAGMENT_TYPES[1] = new FragmentType(2, "test2");
        FragmentType.FRAGMENT_TYPES[2] = new FragmentType(3, "test3");
    }

    public static final FragmentType getInstance(int instanceId) {
        if (instanceId >= FragmentType.FRAGMENT_TYPES.length) {
            throw new RuntimeException("Invalide instance id: " + instanceId);
        }
        return FragmentType.FRAGMENT_TYPES[instanceId];
    }

    public FragmentType(int id, String name) {
        this.id = id;
        this.name = name;
    }
}
