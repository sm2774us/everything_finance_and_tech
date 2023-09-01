/* 
 * Copyright 2023 (c) CoralBlocks - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
package com.coralblocks.coralme.util;

/**
 * This utility class convert doubles with 8-decimal precision to longs and vice-versa. 
 */
public class DoubleUtils {
	
	/**
	 * The precision we are using, which is 8 decimals.
	 */
	public static final int PRECISION = 8;
	
	private static final long MULTIPLIER = (long) Math.pow(10, PRECISION);
	
	/**
	 * Converts a double value to a long, rounding to 8 decimals of precision.
	 * 
	 * @param value the double value to convert to a long
	 * @return the long representing the double value
	 */
	public static long toLong(double value) {
		return Math.round(value * MULTIPLIER);
	}
	
	/**
	 * Converts a long value to a double with 8 decimals of precision.
	 * 
	 * @param value the long value to convert to a double
	 * @return the double representing the long value
	 */
	public static double toDouble(long value) {
		return ((double) value) / ((double) MULTIPLIER);
	}
}
