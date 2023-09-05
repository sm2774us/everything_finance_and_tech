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
 * This utility class provides methods to work with <tt>String</tt>s and <tt>CharSequence</tt>s in an efficient way and without producing any garbage.
 */
public class StringUtils {

	/**
	 * Checks if these two CharSequences represent the same String
	 * 
	 * @param cs1 the first CharSequence
	 * @param cs2 the second CharSequence
	 * @return true if they represent the same String
	 */
	public final static boolean equals(CharSequence cs1, CharSequence cs2) {
		if (cs1.length() == cs2.length()) {
			for (int i = 0; i < cs1.length(); i++) {
				if (cs1.charAt(i) != cs2.charAt(i)) return false;
			}
			return true;
		}
		return false;
	}
}
