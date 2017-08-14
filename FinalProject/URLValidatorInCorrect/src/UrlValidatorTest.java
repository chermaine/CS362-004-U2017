/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;



//Chermaine Cheang (cheangc), Philip Chang(changp), Victor Ness(nessv)

/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   //partitions
   private String[] partitionWithEmptyString = {"", null};
   private String[] partitionWithSpaces = {" ", 
		   	"http://www.google.com/foo/bar ", 
		   	"http://www. google.com/foo/bar", 
		   	"http://  ", 
		   	" http://www.google.com/foo", 
		   	"http://www.google.com/foo /bar",
		   	"ftp:// www.google.com/foo/bar",
		   	"http://www.google .com/foo/bar",
		   	"http://www.google.com /foo/bar",
		   	"http ://www.google.com/foo/bar"
		   };
   
   private String[] partitionWithMorePeriods = {
		   "http://www.google.com/../foo",
		   "http://www.google.com/foo/..",
		   "http://www.google.com../foo",
		   "http://www.google..com/foo",
		   "http://www..google.com/foo",
		   "http://..www.google.com/foo",
		   "http://.www.google.com/foo",
		   "..http://www.google.com/foo",
		   "http://www.google.com/foo..",
		   "http://w.w..w.google.com/foo",
		   "ftp://.......goole.com",
		   "ftp://www.....google....com"
   };
   
   private String[] partitionWithMoreSlashes = {
		   "http://www.google.com//",
		   "http://www.google.com//foo",
		   "http://www.google.com/foo//bar",
		   "http:////www.google.com",
		   "http://www//google.com",
		   "ftp://www.google.com//foo"
   };
   
   private String[] partitionWithQueryParameters = {
		   "http://www.google.com/?action=view",
		   "http://go.com:80/?action=edit&mode=up",
		   "http://www.google.com:65535/test1?action=edit&mode=up",
		   "http://example.com/path/to/page?name=ferret&color=purple",
		   "http://www.google.com:65535/test1?action=edit&mode=up",
		   "http://example.com/over/there?name=ferret",
		   "http://www.google.com/action=view",
		   "http://www.google.com/action=view&mode=up"
   };
   
   private String[] partitionWithValidURL = {
		   "http://www.google.com",
		   "http://www.google.com/foo/bar",
		   "http://www.google.com/foo%20",
		   "ftp://amazon.com",
		   "ftp://0.0.0.0/foo"
   };
   
   private String[] partitionWithInvalidPort = {
		   "http://www.google.com:-8",
		   "http://www.google.com:abc", 
		   "http://www.google.com:12c",
		   "http://www.google.com:#123",
		   "http://www.google.com:..."
   };
   
   private String[] partitionWithNoPeriods = {
            "a",
            "abcd",
            "http://abcd"
   };
   
   private String[] partitionWithInvalidUrlScheme = {
            "a://google.com",
            "abc://google.com",
            "http..://www.google.com",
            "ht..tp://www.google.com",
            "123://google.com",
            "1://google.com",
            "$://google.com",
            "##://google.com",
            "abc##://google.com",
            "#abc://google.com",
            "a b://google.com",
            "# b://google.com",
            "b #://google.com",
            "http:://www.google.com",
            "www.google.com", 
            "http://www.google.com"
            
   };
   private String[] partitionWithoutColonSlashes = {
            "httpgoogle.com",
            "httpsgoogle.com",
            "httpsamazon.com",
            "httpamazon.com",
            "httpwww.google.com"

   };
   public void assertTrue(boolean condition, String test) {
	   if (condition) {
		   System.out.println("INVALID: " + test);
	   }
	   else {
		   System.out.println("VALID: " + test);
	   }
   }
   
   public UrlValidatorTest(String testName) {
      super(testName);
   }

   public void testManualTest()
   {
       System.out.println("\nManual Testing:");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println(urlVal.isValid("http://localhost:80"));
	   assertTrue(urlVal.isValid("http://www.amazon.com/foo/..") == false, "http://www.amazon.com/foo/..");
	   assertTrue(urlVal.isValid("https://amazon") == false, "https://amazon");
	   assertTrue(urlVal.isValid("http://amazon") == false, "http://amazon");
	   assertTrue(urlVal.isValid("https://amazon.com") == true, "https://amazon.com");
	   assertTrue(urlVal.isValid("http://amazon.com") == true, "http://amazon.com");
	   assertTrue(urlVal.isValid("https://www.amazon") == false, "https://www.amazon");
	   assertTrue(urlVal.isValid("http://www.amazon") == false, "http://www.amazon");
	   assertTrue(urlVal.isValid("htps://www.amazon.com") == true, "htps://www.amazon.com");
	   assertTrue(urlVal.isValid("http:/www.amazon.com") == false, "http:/www.amazon.com");
	   assertTrue(urlVal.isValid("http://www.amazon.com/foo/bar%20") == true, "http://www.amazon.com/foo/bar%20");
	   assertTrue(urlVal.isValid("http://www.amazon.com/foo/bar %20") == false, "http://www.amazon.com/foo/bar %20");
	   assertTrue(urlVal.isValid("http://www.amazon.com/foo/&20") == true, "http://www.amazon.com/foo/&20");
	   assertTrue(urlVal.isValid("http://www.google.com:65535/test1?action=edit&mode=up") == true, "http://www.google.com:65535/test1?action=edit&mode=up");
	   assertTrue(urlVal.isValid("http://example.com/over/there?name=ferret") == true, "http://example.com/over/there?name=ferret");
	   assertTrue(urlVal.isValid("http://example.com/path/to/page?name=ferret&color=purple") == true, "http://example.com/path/to/page?name=ferret&color=purple");
	   assertTrue(urlVal.isValid("//example.org/scheme-relative/URI/with/absolute/path/to/resource.txt") == false, "//example.org/scheme-relative/URI/with/absolute/path/to/resource.txt");
	   assertTrue(urlVal.isValid("") == false, "");
	   assertTrue(urlVal.isValid("http://localhost:8080/users") == true, "http://localhost:8080/users");
	   assertTrue(urlVal.isValid("http://www.google.com:65535/test1?action=edit&mode=up") == true, "http://www.google.com:65535/test1?action=edit&mode=up");
   }
   
   //testing partition that contains empty string as URL
   public void testPartitionWithEmptyString()
   {
	   System.out.println("\nTesting URLs with empty string:");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithEmptyString.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithEmptyString[i]) == false, partitionWithEmptyString[i]);
	   }
   }
   
   //testing partition that contains spaces in URL
   public void testPartitionWithSpaces(){
	   System.out.println("\nTesting URLs with spaces:");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithSpaces.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithSpaces[i]) == false, partitionWithSpaces[i]);
	   }
   }
   
   //testing partition that contains more than one periods together in a URL
   public void testPartitionWithMorePeriods() {
	   System.out.println("\nTesting URLs with more than one periods together");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithMorePeriods.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithMorePeriods[i]) == false, partitionWithMorePeriods[i]);
	   }
   }
   
 //testing partition that contains more than one slashes together in a URL
   public void testPartitionWithMoreSlashes() {
	   System.out.println("\nTesting URLs with more than one slashes together");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithMoreSlashes.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithMoreSlashes[i]) == false, partitionWithMoreSlashes[i]);
	   }
   }
   
   //testing partition that contains query parameters in a URL
   public void testPartitionWithQueries() {
	   System.out.println("\nTesting URLs with queries");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithQueryParameters.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithQueryParameters[i]) == false, partitionWithQueryParameters[i]);
	   }
   }
   
 //testing partition that contains query parameters in a URL
   public void testPartitionWithValidURL() {
	   System.out.println("\nTesting valid URLs");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithValidURL.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithValidURL[i]) == false, partitionWithValidURL[i]);
	   }
   }
   
    public void testPartitionWithNoPeriods() {
        System.out.println("\nTesting valid URLs");
        UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
        for (int i = 0; i < partitionWithNoPeriods.length; i++) {
            assertTrue(urlVal.isValid(partitionWithNoPeriods[i]) == false, partitionWithNoPeriods[i]);
        }
    }

   public void testPartitionWithInvalidPort() {
	   System.out.println("\nTesting URLs with invalid port");
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   for (int i = 0; i < partitionWithInvalidPort.length; i++) {
		   assertTrue(urlVal.isValid(partitionWithInvalidPort[i]) == false, partitionWithInvalidPort[i]);
	   }
   }
   //test that checkks partition with invalid url scheme
   public void testPartitionWithInvalidUrlScheme(){
       System.out.println("\nTesting URls with invalid URL scheme");
       UrlValidator urlVal = new UrlValidator(null,null, UrlValidator.ALLOW_ALL_SCHEMES);
       for (int i = 0; i < partitionWithInvalidUrlScheme.length; i++){
           assertTrue(urlVal.isValid(partitionWithInvalidUrlScheme[i]) == false, partitionWithInvalidUrlScheme[i]);
       }
   }
   public void testPartitionWithoutColonSlashe(){
       System.out.println("\nTesting URls without ://");
       UrlValidator urlVal = new UrlValidator(null,null, UrlValidator.ALLOW_ALL_SCHEMES);
       for (int i = 0; i < partitionWithoutColonSlashes.length; i++){
           assertTrue(urlVal.isValid(partitionWithoutColonSlashes[i]) == false, partitionWithoutColonSlashes[i]);
       }
   }
   /**
    * Create set of tests by taking the testUrlXXX arrays and
    * running through all possible permutations of their combinations.
    *
    * @param testObjects Used to create a url.
    */
   

}
