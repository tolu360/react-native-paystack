package com.arttitude360.reactnativepaystack.modules;

import android.app.Activity;
import android.util.Log;
import android.net.Uri;

import co.paystack.android.Paystack;
import co.paystack.android.PaystackSdk;
import co.paystack.android.model.Card;
import co.paystack.android.model.Token;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.facebook.react.bridge.Callback;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;

import java.lang.reflect.InvocationTargetException;

public class PaystackAndroid extends ReactContextBaseJavaModule {

	protected Token token;
	protected Card card;

	public static final String TAG = "PaystackAndroid";

	protected Callback successCallback;
	protected Callback errorCallback;

	@Override
    public String getName() {
        return "PaystackAndroid";
    }

    Activity mActivity;

    public PaystackAndroid(
            ReactApplicationContext reactContext,
            Activity activity) {
        super(reactContext);
        mActivity = activity;

        //initialize sdk
    	PaystackSdk.initialize(getReactApplicationContext());
    }

    protected void handleError(String errorMsg){
        try {
            Log.e(TAG, errorMsg);
            JSONObject error = new JSONObject();
            error.put("error", errorMsg);
            errorCallback.invoke(error);
        } catch (JSONException e) {
            Log.e(TAG, e.toString());
        }
    }

    protected void handleSuccess(String msg){
        try {
            Log.i(TAG, msg);
            JSONObject success = new JSONObject();
            success.put("token", msg);
            successCallback.invoke(success);
        } catch (JSONException e) {
            handleError(e.getMessage());
        }
    }

    @ReactMethod
    public void getToken(String cardNumber, int expiryMonth, int expiryYear, String cvc, Callback errorCb, Callback successCb) {
    	
		errorCallback = errorCb;
		successCallback = successCb;		

		//check card validity
        validateCard(cardNumber, expiryMonth, expiryYear, cvc);
		
		if (card.isValid()) {
			createToken(card);
		}
    }

    protected void validateCard(String cardNumber, int expiryMonth, int expiryYear, String cvc) {

		if (isEmpty(cardNumber)) {
			handleError("Empty card number");
			return;
		}

		//build card object with ONLY the number, update the other fields later
		card = new Card.Builder(cardNumber, 0, 0, "").build();

		if (!card.validNumber()) {
			handleError("Invalid card number");
			return;
		}

		//validate cvc
		if (isEmpty(cvc)) {
			handleError("Empty cvc");
			return;
		}
		
		//update the cvc field of the card
		card.setCvc(cvc);

		//check that it's valid
		if (!card.validCVC()) {
			handleError("Invalid cvc");
			return;
		}

		//validate expiry month
		if (expiryMonth < 1) {
			handleError("Invalid month");
			return;
		}

		//update the expiryMonth field of the card
		card.setExpiryMonth(expiryMonth);

		//validate expiry year		
		if (expiryYear < 1) {
			handleError("Invalid year");
			return;
		}

		//update the expiryYear field of the card
		card.setExpiryYear(expiryYear);

		//validate expiry
		if (!card.validExpiryDate()) {
			handleError("Invalid expiry");
		}
    }

	private void createToken(Card card) {
		//then create token using PaystackSdk class
		PaystackSdk.createToken(card, new Paystack.TokenCallback() {
			@Override
			public void onCreate(Token token) {
				//here you retrieve the token, and send to your server for charging.
				handleSuccess(token.token);
			
			}

			@Override
			public void onError(Exception error) {
				handleError(error.getMessage());
			}
		});
	}

	private boolean isEmpty(String s) {
		return s == null || s.length() < 1;
	}
       
}