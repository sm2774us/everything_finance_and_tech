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


public class StringUtilsTest {
	
	@Test
	public void test1() {
		
		String s = "house";
		
		CharSequence cs1 = new StringBuilder(s);
		CharSequence cs2 = new StringBuilder(s);
		
		Assert.assertFalse(cs1.equals(cs2)); // yes, they are not equal, don't ask me why
		Assert.assertFalse(cs1.equals(s)); // expected... (different types)
		Assert.assertFalse(s.equals(cs1)); // expected... (different types)
		
		// now magic:
		
		Assert.assertTrue(StringUtils.equals(cs1, cs2));
		Assert.assertTrue(StringUtils.equals(cs1, s));
		Assert.assertTrue(StringUtils.equals(s, cs1));
	}
}