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

public class DoubleUtilsTest {
	
	@Test
	public void test1() {
		
		double d1 = 3.13222;
		long l1 = DoubleUtils.toLong(d1);
		
		double d2 = DoubleUtils.toDouble(l1);
		
		Assert.assertTrue(d1 == d2);
	}
	
	@Test
	public void test2() {
		
		long l1 = 123123123;
		double d1 = DoubleUtils.toDouble(l1);
		
		long l2 = DoubleUtils.toLong(d1);
		
		Assert.assertTrue(l1 == l2);
	}
}