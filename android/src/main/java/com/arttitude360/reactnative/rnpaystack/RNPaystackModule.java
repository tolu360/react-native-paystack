package com.arttitude360.reactnative.rnpaystack;

import android.app.Activity;
import android.util.Log;
import android.net.Uri;
import android.os.Bundle;
import android.content.Intent;
import android.util.Patterns;

import co.paystack.android.Paystack;
import co.paystack.android.PaystackSdk;
import co.paystack.android.model.Card;
import co.paystack.android.model.Token;
import co.paystack.android.model.Charge;
import co.paystack.android.model.Transaction;

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
    private Charge charge;
    private Transaction transaction;

    private ReactApplicationContext reactContext;
    private Promise pendingPromise;
    private ReadableMap chargeOptions;

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

    protected WritableMap buildSuccessMsg(String token, String lastDigits) {
        WritableMap successData = new WritableNativeMap();
        successData.putString("token", token);
        successData.putString("last4", lastDigits);
        return successData;
    }

    @ReactMethod
    public void getToken(ReadableMap cardData, final Promise promise) {
        
        this.pendingPromise = promise;

        String cardNumber = cardData.getString("cardNumber");
        String expiryMonth = cardData.getString("expiryMonth");
        String expiryYear = cardData.getString("expiryYear");
        String cvc = cardData.getString("cvc");

        validateCard(cardNumber, expiryMonth, expiryYear, cvc);
        
        if (card != null && card.isValid()) {
            createToken(card);
        } 
    }

    @ReactMethod
    public void chargeCard(ReadableMap cardData, final Promise promise) {

        this.chargeOptions = null;
        
        this.pendingPromise = promise;
        this.chargeOptions = cardData;

        validateTransaction();

        if (card != null && card.isValid()) {
            try {
                createTransaction();
            } catch(Exception error) {
                rejectPromise("E_CHARGE_ERROR", error.getMessage());
            }
            
        }
    }

    private void validateCard(String cardNumber, String expiryMonth, String expiryYear, String cvc) {

        if (isEmpty(cardNumber)) {
            rejectPromise("E_INVALID_NUMBER", "Empty card number");
            return;
        }

        //build card object with ONLY the number, update the other fields later
        card = new Card.Builder(cardNumber, 0, 0, "").build();

        if (!card.validNumber()) {
            rejectPromise("E_INVALID_NUMBER", "Invalid card number");
            return;
        }

        //validate cvc
        if (isEmpty(cvc)) {
            rejectPromise("E_INVALID_CVC", "Empty CVC");
            return;
        }
        
        //update the cvc field of the card
        card.setCvc(cvc);

        //check that it's valid
        if (!card.validCVC()) {
            rejectPromise("E_INVALID_CVC", "Invalid CVC");
            return;
        }

        int month = -1;
        try {
            month = Integer.parseInt(expiryMonth);
        } catch (Exception ignored) {
        }

        //validate expiry month
        if (month < 1) {
            rejectPromise("E_INVALID_MONTH", "Invalid expiration month");
            return;
        }

        //update the expiryMonth field of the card
        card.setExpiryMonth(month);

        int year = -1;
        try {
            year = Integer.parseInt(expiryYear);
        } catch (Exception ignored) {
        }

        //validate expiry year      
        if (year < 1) {
            rejectPromise("E_INVALID_YEAR", "Invalid expiration year");
            return;
        }

        //update the expiryYear field of the card
        card.setExpiryYear(year);

        //validate expiry
        if (!card.validExpiryDate()) {
            rejectPromise("E_INVALID_DATE", "Invalid expiration date");
            return;
        }
    }

    private void validateTransaction() {
        
        String cardNumber = chargeOptions.getString("cardNumber");
        String expiryMonth = chargeOptions.getString("expiryMonth");
        String expiryYear = chargeOptions.getString("expiryYear");
        String cvc = chargeOptions.getString("cvc");
        String email = chargeOptions.getString("email");
        int amountInKobo = chargeOptions.getInt("amountInKobo");

        validateCard(cardNumber, expiryMonth, expiryYear, cvc);

        charge = new Charge();
        charge.setCard(card);

        if (isEmpty(email)) {
            rejectPromise("E_INVALID_EMAIL", "Email cannot be empty");
            return;
        }

        if (!Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
            rejectPromise("E_INVALID_EMAIL", "Invalid email");
            return;
        }

        charge.setEmail(email);

        if (amountInKobo < 1) {
            rejectPromise("E_INVALID_AMOUNT", "Invalid amount");
            return;
        }

        charge.setAmount(amountInKobo);

        if (hasStringKey("currency")) {
            charge.setCurrency(chargeOptions.getString("currency"));
        }

        if (hasStringKey("plan")) {
            charge.setPlan(chargeOptions.getString("plan"));
        }

        if (hasStringKey("subAccount")) {
            charge.setSubaccount(chargeOptions.getString("subAccount"));

            if (hasStringKey("bearer") && chargeOptions.getString("bearer") == "subaccount") {
                charge.setBearer(Charge.Bearer.subaccount);
            }

            if (hasStringKey("bearer") && chargeOptions.getString("bearer") == "account") {
                charge.setBearer(Charge.Bearer.account);
            }

            if (hasIntKey("transactionCharge")) {
                charge.setTransactionCharge(chargeOptions.getInt("transactionCharge"));
            }
        }

        if (hasStringKey("reference")) {
            charge.setReference(chargeOptions.getString("reference"));
        }

    }

    private void createToken(Card card) {

        //then create token using PaystackSdk class
        PaystackSdk.createToken(card, new Paystack.TokenCallback() {
            @Override
            public void onCreate(Token token) {
                //here you retrieve the token, and send to your server for charging.
                resolvePromise(buildSuccessMsg(token.token, token.last4));      
            }

            @Override
            public void onError(Throwable error) {
                rejectPromise("E_TOKEN_ERROR", error.getMessage());
            }
        });
    }

    private void createTransaction() {
        
        transaction = null;
        Activity currentActivity = getCurrentActivity();

        PaystackSdk.chargeCard(currentActivity, charge, new Paystack.TransactionCallback() {
            @Override
            public void onSuccess(Transaction transaction) {
                
                // This is called only after transaction is successful
                RNPaystackModule.this.transaction = transaction;

                WritableMap map = Arguments.createMap();
                map.putString("reference", transaction.reference);

                resolvePromise(map);
            }

            @Override
            public void beforeValidate(Transaction transaction) {
                // This is called only before requesting OTP
                // Save reference so you may send to server if
                // error occurs with OTP
                RNPaystackModule.this.transaction = transaction;
            }

            @Override
            public void onError(Throwable error) {
               
                if (RNPaystackModule.this.transaction == null) {
                    rejectPromise("E_TRANSACTION_ERROR", error.getMessage());
                } else {
                    rejectPromise("E_TRANSACTION_ERROR", transaction.reference + " concluded with error: " + error.getMessage());
                }
            }

        });
    }

    private boolean isEmpty(String s) {
        return s == null || s.length() < 1;
    }

    private boolean hasStringKey(String key) {
        return chargeOptions.hasKey(key) && !chargeOptions.isNull(key) && !chargeOptions.getString(key).isEmpty();
    }

    private boolean hasIntKey(String key) {
        return chargeOptions.hasKey(key) && !chargeOptions.isNull(key) && chargeOptions.getInt(key) > 0;
    }

    private void rejectPromise(String code, String message) {
        if (this.pendingPromise != null) {
            this.pendingPromise.reject(code, message);
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