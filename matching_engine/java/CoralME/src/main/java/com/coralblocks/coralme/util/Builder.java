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
 * Creates new instances of a given type. A builder is useful for object pools that need to instantiate new objects on demand and/or during startup. 
 *
 * @param <E> the type of objects this builder produces
 */
public interface Builder<E> {

	/**
	 * Creates a fresh new instance of the given type. Note that a builder must never return the same instance twice.
	 * 
	 * @return a fresh new instance
	 */
	public E newInstance();
}