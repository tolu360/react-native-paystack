package com.arttitude360.reactnative.rnpaystack;

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

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.WritableNativeMap;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;

public class RNPaystackModule extends ReactContextBaseJavaModule {

	protected Token token;
	protected Card card;

	private ReactApplicationContext reactContext;
    private Promise pendingPromise;

	public static final String TAG = "RNPaystack";

	public static String REACT_CLASS = "RNPaystack";

    public RNPaystackModule(ReactApplicationContext reactContext) {
        super(reactContext);

        this.reactContext = reactContext;

        // Initialize PaystackSdk
    	PaystackSdk.initialize(this.reactContext);
    }

    @Override
    public String getName() {
        return REACT_CLASS;
    }

    protected WritableMap buildSuccessMsg(String token, String lastDigits){
    	WritableMap successData = new WritableNativeMap();
		successData.putString("token", token);
		successData.putString("last4", lastDigits);
		return successData;
    }

    @ReactMethod
    public void getToken(String cardNumber, String expiryMonth, String expiryYear, String cvc, final Promise promise) {
    	
    	this.pendingPromise = promise;

		if (isEmpty(cardNumber.trim())) {
			rejectPromise("E_INVALID_NUMBER", "Empty card number");
			return;
		}

		//build card object with ONLY the number, update the other fields later
		card = new Card.Builder(cardNumber.trim(), 0, 0, "").build();

		if (!card.validNumber()) {
			rejectPromise("E_INVALID_NUMBER", "Invalid card number");
			return;
		}

		//validate cvc
		if (isEmpty(cvc.trim())) {
			rejectPromise("E_INVALID_CVC", "Empty CVC");
			return;
		}
		
		//update the cvc field of the card
		card.setCvc(cvc.trim());

		//check that it's valid
		if (!card.validCVC()) {
			rejectPromise("E_INVALID_CVC", "Invalid CVC");
			return;
		}

		int month = -1;
        try {
            month = Integer.parseInt(expiryMonth.trim());
        } catch (Exception ignored) {
        }

		//validate expiry month
		if (month < 1) {
			rejectPromise("E_INVALID_MONTH", "Invalid expiration month");
			return;
		}

		//update the expiryMonth field of the card
		card.setExpiryMonth(expiryMonth.trim());

		int year = -1;
        try {
            year = Integer.parseInt(expiryYear.trim());
        } catch (Exception ignored) {
        }

		//validate expiry year		
		if (year < 1) {
			rejectPromise("E_INVALID_YEAR", "Invalid expiration year");
			return;
		}

		//update the expiryYear field of the card
		card.setExpiryYear(expiryYear.trim());

		//validate expiry
		if (!card.validExpiryDate()) {
			rejectPromise("E_INVALID_DATE", "Invalid expiration date");
			return;
		}
		
		if (card.isValid()) {
			createToken(card, promise);
		} 
    }

	private void createToken(Card card, final Promise promise) {
		this.pendingPromise = promise;

		//then create token using PaystackSdk class
		PaystackSdk.createToken(card, new Paystack.TokenCallback() {
			@Override
			public void onCreate(Token token) {
				//here you retrieve the token, and send to your server for charging.
				resolvePromise(buildSuccessMsg(token.token, token.last4));		
			}

			@Override
			public void onError(Exception error) {
				rejectPromise("E_TOKEN_ERROR", "Error retrieving token for card");
			}
		});
	}

	private boolean isEmpty(String s) {
		return s == null || s.length() < 1;
	}

	private void rejectPromise(String type, String message) {
        if (this.pendingPromise != null) {
            this.pendingPromise.reject(type, message);
            this.pendingPromise = null;
        }
    }

    private void resolvePromise(Object data) {
        if (this.pendingPromise != null) {
            this.pendingPromise.resolve(data);
            this.pendingPromise = null;
        }
    }
       
}