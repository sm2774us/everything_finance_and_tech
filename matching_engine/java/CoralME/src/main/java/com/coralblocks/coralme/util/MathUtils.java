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
 * This utility class provides methods to work with mathematics in an efficient way and without producing any garbage. 
 */
public class MathUtils {
	
	/**
	 * Is this long a power of two?
	 * 
	 * @param l the long
	 * @return true if it is a power of two
	 */
	public final static boolean isPowerOfTwo(long l) {
	    return (l & (l - 1)) == 0;
	}
	
	/**
	 * Throws an IllegalArgumentException if this long is not a power of two
	 * 
	 * @param number the long
	 */
	public final static void ensurePowerOfTwo(long number) {
		if (!isPowerOfTwo(number)) {
			throw new IllegalArgumentException("Not a power of two: " + number);
		}
	}
}

