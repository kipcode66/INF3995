package ca.polymtl.inf3995.team1.tp4;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.View;

public class MainActivity extends FragmentActivity implements RequestButtonPageFragment.OnFragmentInteractionListener {
    public static final int NUM_PAGES = 3;
    public static final int NUM_LOOPS = 10000;

    private ViewPager mPager;
    private PagerAdapter mPagerAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Instantiate a ViewPager and a PagerAdapter.
        mPager = (ViewPager) findViewById(R.id.pager);
        mPagerAdapter = new RequestButtonPagerAdapter(getSupportFragmentManager());
        mPager.setAdapter(mPagerAdapter);
        mPager.setCurrentItem(NUM_PAGES * NUM_LOOPS / 2);
    }

    /**
     * Handler of a click of the button.
     * @param view
     * @param type An instance of {@link RequestData} containing data about the request.
     */
    @Override
    public void onFragmentInteraction(View view, RequestData type) {
        Intent intent = new Intent(MainActivity.this, RequestActivity.class);
        Bundle b = new Bundle();
        b.putString("requestUrl", type.requestUrl);
        intent.putExtras(b);
        startActivity(intent);
    }

    /**
     * A simple pager adapter that represents 3 RequestButtonPageFragment objects, in
     * sequence.
     */
    private class RequestButtonPagerAdapter extends FragmentStatePagerAdapter {
        public RequestButtonPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            return (Fragment) RequestButtonPageFragment.newInstance(RequestData.getInstance(position % NUM_PAGES));
        }

        @Override
        public int getCount() {
            return NUM_PAGES * NUM_LOOPS;
        }
    }
}
