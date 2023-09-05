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
import java.util.NoSuchElementException;

/**
 * A hash map that uses a single primitive char as keys and is backed up by an array with 256 elements for super fast access.
 * 
 * <p><b>NOTE:</b> This data structure is designed on purpose to be used by <b>single-threaded systems</b>, in other words, 
 *  it will break if used concurrently by multiple threads.</p>
 *    
 * @param <E> the entry type this hash map will hold
 */
public final class CharMap<E> implements Iterable<E> {

	@SuppressWarnings("unchecked")
	private final E[] data = (E[]) new Object[256];
	
	private int count = 0;
	private final ReusableIterator iter = new ReusableIterator();
	private int currIteratorKey;
	
	/**
	 * Creates a new CharMap instance
	 */
	public CharMap() {
		
	}

	private final int convert(char key) {
		return ((byte) key) & 0xff;
	}
	
	/**
	 * Does the map contain the given key?
	 * 
	 * @param key the key to check
	 * @return true if the map contains the given key
	 */
	public final boolean containsKey(char key) {
		return data[convert(key)] != null;
	}

	/**
	 * Adds a value for the given key in this map
	 * 
	 * @param key the key to add
	 * @param value the value to add
	 * @return any previous value associated with the given key or null if there was not a value associated with this key
	 */
	public final E put(char key, E value) {
		
		if (value == null) {
			throw new NullPointerException("CharMap does not support NULL values: " + key);
		}

		int index = convert(key);
		E old = data[index];
		data[index] = value;
		if (old == null) {
			// not replacing...
			count++;
		}
		return old;
	}
	
	/**
	 * When using the Iterator for this CharMap, this method will return the current key of the last 
	 * element returned by Iterator.next().
	 * 
	 * @return the current key of the last iterated element
	 */
	public char getCurrIteratorKey() {
		return (char) currIteratorKey;
	}

	/**
	 * Returns the value associated with the given key in this map
	 * 
	 * @param key the key to get the value
	 * @return the value associated with the given key
	 */
	public final E get(char key) {
		return data[convert(key)];
	}

	/**
	 * Removes and returns the value associated with the given key in the map
	 * 
	 * @param key the key to remove
	 * @return the value for the removed key
	 */
	public final E remove(char key) {
		int index = convert(key);
		E old = data[index];
		data[index] = null;
		if (old != null) {
			// really removing something...
			count--;
		}
		return old;
	}

	private class ReusableIterator implements Iterator<E> {

		int index = 0;
		int position = 0;
		int size;

		public void reset() {
			this.index = 0;
			this.position = 0;
			this.size = count;
			currIteratorKey = 0;
		}

		@Override
		public final boolean hasNext() {
			return position < size;
		}

		@Override
		public final E next() {
			
			if (position >= size) {
				throw new NoSuchElementException();
			}
			
			E e = null;
			while(e == null) {
				e = data[index];
				index++;
			}
			currIteratorKey = index - 1;
			position++;
			return e;
		}

		@Override
		public void remove() {
			if (index == 0 || data[index - 1] == null) {
				throw new NoSuchElementException();
			}
			data[index - 1] = null;
			count--;
		}
	}

	/**
	 * Returns the same instance of the iterator (garbage-free)
	 * 
	 * @return the same instance of the iterator
	 */
	@Override
	public final Iterator<E> iterator() {
		iter.reset();
		return iter;
	}

	/**
	 * Is this map empty? (size == 0)
	 * 
	 * @return true if empty
	 */
	public final boolean isEmpty() {
		return count == 0;
	}

	/**
	 * Clears the map. The map will be empty (size == 0) after this operation.
	 */
	public final void clear() {
		for (int i = 0; i < data.length; i++) {
			data[i] = null;
		}
		count = 0;
	}

	/**
	 * Returns the size of this map
	 * 
	 * @return the size of this map
	 */
	public final int size() {
		return count;
	}
}
