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

import org.junit.Assert;
import org.junit.Test;

public class MathUtilsTest {
	
	@Test
	public void testSeveral() {
		
		for(int i = 0; i < 10; i++) {
			long x = (long) Math.pow(2, i);
			Assert.assertEquals(true, MathUtils.isPowerOfTwo(x));
		}
	}
	
	@Test
	public void testFalse() {

		Assert.assertEquals(false, MathUtils.isPowerOfTwo(3));
		Assert.assertEquals(false, MathUtils.isPowerOfTwo(33));
		
		long x = (long) Math.pow(2, 11);
		Assert.assertEquals(false, MathUtils.isPowerOfTwo(x + 1));
	}
	
	@Test
	public void testLargest() {
		long x = (long) Math.pow(2, 62);
		Assert.assertEquals(true, MathUtils.isPowerOfTwo(x));
	}
}