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
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.bridge.WritableNativeMap;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;

public class PaystackAndroid extends ReactContextBaseJavaModule {

	protected Token token;
	protected Card card;

	public static final String TAG = "PaystackAndroid";

	@Override
    public String getName() {
        return "PaystackAndroid";
    }

    public PaystackAndroid(
            ReactApplicationContext reactContext) {
        super(reactContext);

        //initialize sdk
    	PaystackSdk.initialize(getReactApplicationContext());
    }

    protected WritableMap buildErrorMsg(String errorMsg, int errorCode) {
    	WritableMap errorData = new WritableNativeMap();
		errorData.putString("error", errorMsg);
		errorData.putInt("code", errorCode);
		return errorData;        
    }

    protected WritableMap buildSuccessMsg(String token, String lastDigits){
    	WritableMap successData = new WritableNativeMap();
		successData.putString("token", token);
		successData.putString("last4", lastDigits);
		return successData;
    }

    @ReactMethod
    public void getToken(String cardNumber, int expiryMonth, int expiryYear, String cvc, Callback errorCb, Callback successCb) {
    	
		if (isEmpty(cardNumber)) {
			errorCb.invoke(buildErrorMsg("Empty card number.", 420));
			return;
		}

		//build card object with ONLY the number, update the other fields later
		card = new Card.Builder(cardNumber, 0, 0, "").build();

		if (!card.validNumber()) {
			errorCb.invoke(buildErrorMsg("Invalid card number.", 421));
			return;
		}

		//validate cvc
		if (isEmpty(cvc)) {
			errorCb.invoke(buildErrorMsg("Empty cvc.", 422));
			return;
		}
		
		//update the cvc field of the card
		card.setCvc(cvc);

		//check that it's valid
		if (!card.validCVC()) {
			errorCb.invoke(buildErrorMsg("Invalid cvc.", 423));
			return;
		}

		//validate expiry month
		if (expiryMonth < 1) {
			errorCb.invoke(buildErrorMsg("Invalid expiration month.", 424));
			return;
		}

		//update the expiryMonth field of the card
		card.setExpiryMonth(expiryMonth);

		//validate expiry year		
		if (expiryYear < 1) {
			errorCb.invoke(buildErrorMsg("Invalid expiration year.", 425));
			return;
		}

		//update the expiryYear field of the card
		card.setExpiryYear(expiryYear);

		//validate expiry
		if (!card.validExpiryDate()) {
			errorCb.invoke(buildErrorMsg("Invalid expiration date", 426));
		}
		
		if (card.isValid()) {
			createToken(card, errorCb, successCb);
		} 
    }

	private void createToken(Card card, final Callback errorCb, final Callback successCb) {
		//then create token using PaystackSdk class
		PaystackSdk.createToken(card, new Paystack.TokenCallback() {
			@Override
			public void onCreate(Token token) {
				//here you retrieve the token, and send to your server for charging.
				successCb.invoke(buildSuccessMsg(token.token, token.last4));			
			}

			@Override
			public void onError(Exception error) {
				errorCb.invoke(buildErrorMsg(error.getMessage(), 427));
			}
		});
	}

	private boolean isEmpty(String s) {
		return s == null || s.length() < 1;
	}
       
}