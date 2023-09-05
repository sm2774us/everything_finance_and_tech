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

import java.util.Iterator;

import org.junit.Assert;
import org.junit.Test;

/*
 * Copied from CharMapTest
 */
public class LongMapTest {
	
	@Test
	public void test1() {
		
		LongMap<String> map = new LongMap<String>();
		
		map.put('c', "C");
		
		Assert.assertEquals(1, map.size());
		
		map.put('d', "D");
		
		Assert.assertEquals(2, map.size());
		
		String ret = map.put('d', "DD");
		
		Assert.assertEquals(2, map.size());
		Assert.assertEquals("D", ret);
		
		map.put('a', "A");
		Assert.assertEquals(3, map.size());
		
		ret = map.remove('a');
		
		Assert.assertEquals(2, map.size());
		Assert.assertEquals("A", ret);
		
		Assert.assertEquals(true, map.containsKey('c'));
		Assert.assertEquals(false, map.containsKey('a'));
		
		map.clear();
		
		Assert.assertEquals(true, map.isEmpty());
	}
	
	@Test
	public void test2() {
		
		LongMap<String> map = new LongMap<String>();
		
		map.put('a', "A");
		map.put('b', "B");
		map.put('c', "C");
		map.put('d', "D");
		map.put('e', "E");
		
		map.remove('c');
		
		Assert.assertEquals(4, map.size());
		
		int count = 0;
		
		Iterator<String> iter = map.iterator();
		while(iter.hasNext()) {
			String value = iter.next();
			char key = (char) map.getCurrIteratorKey();
			Assert.assertEquals(value, String.valueOf(key).toUpperCase());
			count++;
		}
		
		Assert.assertEquals(4, count);
		
		count = 0;
		
		iter = map.iterator();
		while(iter.hasNext()) {
			String value = iter.next();
			char key = (char) map.getCurrIteratorKey();
			Assert.assertEquals(value, String.valueOf(key).toUpperCase());
			if (key == 'b' || key == 'd') iter.remove();
			count++;
		}
		
		Assert.assertEquals(4, count);
		Assert.assertEquals(2, map.size());
	}
}