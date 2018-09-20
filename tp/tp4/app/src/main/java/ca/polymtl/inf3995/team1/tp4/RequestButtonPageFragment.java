package ca.polymtl.inf3995.team1.tp4;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link RequestButtonPageFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link RequestButtonPageFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class RequestButtonPageFragment extends Fragment {
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_REQUEST_ID = "requestId";

    private int requestId;
    private RequestData data;

    private OnFragmentInteractionListener mListener;

    public RequestButtonPageFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param data Parameter 1.
     * @return A new instance of fragment RequestButtonPageFragment.
     */
    public static RequestButtonPageFragment newInstance(RequestData data) {
        RequestButtonPageFragment fragment = new RequestButtonPageFragment();
        Bundle args = new Bundle();
        args.putInt(ARG_REQUEST_ID, data.id);
        fragment.setArguments(args);
        fragment.data = data;
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            requestId = getArguments().getInt(ARG_REQUEST_ID);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_request_button_page, container, false);

        Button button = (Button)view.findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                RequestButtonPageFragment.this.onButtonPressed(v);
            }
        });

        button.setText(data.buttonText);

        return view;
    }

    public void onButtonPressed(View view) {
        Intent intent = new Intent();

        if (mListener != null) {
            mListener.onFragmentInteraction(view, data);
        }
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnFragmentInteractionListener) {
            mListener = (OnFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        void onFragmentInteraction(View view, RequestData type);
    }
}
