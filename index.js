
import React from 'react'

import { NativeModules, Platform } from 'react-native'

const RNPaystackNative = NativeModules.RNPaystack

class RNPaystack {
	chargeCard(chargeParams) {
		if (typeof chargeParams != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}

		return RNPaystackNative.chargeCard(chargeParams);
	}

	chargeCardWithAccessCode(chargeParams) {
		if (typeof chargeParams != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}

		return RNPaystackNative.chargeCardWithAccessCode(chargeParams);
	}
}

export default new RNPaystack()