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

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.junit.Assert;
import org.junit.Test;


public class LinkedObjectPoolTest {
	
	@Test
	public void testSameInstance() {
		
		LinkedObjectPool<StringBuilder> pool = new LinkedObjectPool<StringBuilder>(8, StringBuilder.class);
		
		Assert.assertEquals(8, pool.size());
		
		StringBuilder sb1 = pool.get();
		
		Assert.assertEquals(7, pool.size());
		
		pool.release(sb1);
		
		Assert.assertEquals(8, pool.size());
		
		StringBuilder sb2 = pool.get();
		
		Assert.assertEquals(7, pool.size());
		
		Assert.assertTrue(sb1 == sb2); // has to be same instance
		
		StringBuilder sb3 = pool.get();
		StringBuilder sb4 = pool.get();
		
		Assert.assertEquals(5, pool.size());
		
		pool.release(sb4);
		pool.release(sb3);
		
		Assert.assertEquals(7, pool.size());
		
		StringBuilder sb5 = pool.get();
		StringBuilder sb6 = pool.get();
		
		Assert.assertEquals(5, pool.size());
		
		// pool is LIFO (stack)
		Assert.assertTrue(sb5 == sb3);
		Assert.assertTrue(sb6 == sb4);
	}
	
	@Test
	public void testRunOutOfInstances() {
		
		LinkedObjectPool<StringBuilder> pool = new LinkedObjectPool<StringBuilder>(2, StringBuilder.class);
		
		Set<StringBuilder> set = new HashSet<StringBuilder>(2);
		
		Assert.assertEquals(2, pool.size());
		
		set.add(pool.get());
		set.add(pool.get());
		
		Assert.assertEquals(0, pool.size());
		
		StringBuilder sb = pool.get();
		Assert.assertNotEquals(null, sb);
		
		Assert.assertEquals(false, set.contains(sb));
		
		Assert.assertEquals(0, pool.size());
		
		pool.release(sb);
		
		Iterator<StringBuilder> iter = set.iterator();
		while(iter.hasNext()) pool.release(iter.next());
		
		Assert.assertEquals(3, pool.size()); // pool has grown from initial 2 to 3
		
		StringBuilder sb1 = pool.get();
		StringBuilder sb2 = pool.get();
		
		Assert.assertEquals(1, pool.size());
		
		Assert.assertEquals(true, set.contains(sb1));
		Assert.assertEquals(true, set.contains(sb2));
		Assert.assertEquals(false, set.contains(pool.get()));
		
		Assert.assertEquals(0, pool.size());
	}
	
	@Test
	public void testIncreasingPoolSize() {
		
		LinkedObjectPool<StringBuilder> pool = new LinkedObjectPool<StringBuilder>(2, StringBuilder.class);
		
		Assert.assertEquals(2, pool.size());
		
		for(int i = 0; i < 2; i++) pool.release(new StringBuilder());
		
		Assert.assertEquals(4, pool.size());
		
		for(int i = 0; i < 4; i++) pool.get();
		
		Assert.assertEquals(0, pool.size());
	}
	
	@Test
	public void testLIFOForGoodCaching() {
		
		LinkedObjectPool<StringBuilder> pool = new LinkedObjectPool<StringBuilder>(2, StringBuilder.class);
		
		Assert.assertEquals(2, pool.size());
		
		StringBuilder sb1 = pool.get();
		pool.release(sb1);
		
		StringBuilder sb2 = pool.get();
		Assert.assertTrue(sb1 == sb2); 
	}
	
	
}