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
 * A contract for enums so that they can return and be represented by a single character 
 */
public interface CharEnum {
	
	/**
	 * Returns the character associated with this enum. 
	 * The enum implementation must enforce that different enums will return different characters.
	 * In other words, all enums will have unique characters.
	 * 
	 * @return the unique character for this enum
	 */
	public char getChar();
	
}
