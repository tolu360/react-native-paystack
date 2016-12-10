
import React from 'react'

import { NativeModules, Platform } from 'react-native'

const RNPaystackNative = NativeModules.RNPaystack

class RNPaystack {
	getToken(cardParams) {
		if (typeof cardParams != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}

		return RNPaystackNative.getToken(cardParams);
	}

	chargeCard(chargeParams) {
		if (typeof chargeParams != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}

		return RNPaystackNative.chargeCard(chargeParams);
	}
}

export default new RNPaystack()