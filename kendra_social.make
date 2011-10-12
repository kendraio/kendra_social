; -------------
; Kendra Social
; drush makefile
; www.kendra.org
; -------------
; Core version
; -------------

api = 2
core = 7.x
projects[drupal][type] = core

; require Kendra Social core makefile
includes[kendra_social] = https://raw.github.com/kendrainitiative/kendra_social/master/drupal-org.make

; -------------
; Modules
; -------------

projects[feeds_jsonpath_parser][type] = module
;projects[feeds_jsonpath_parser][patch][] = http://drupal.org/files/feeds_jsonpath_parser-drush_make.patch
;projects[feeds_jsonpath_parser][version] = 1.x-dev
projects[feeds_jsonpath_parser][subdir] = contrib
projects[feeds_jsonpath_parser][download][type] = file
projects[feeds_jsonpath_parser][download][url] = http://www.kendra.org.uk/feeds_jsonpath_parser-patched.tgz

; -------------
; Themes
; -------------

; -------------
; Patches
; -------------

; Notices on admin/index
; http://drupal.org/node/999582
projects[rubik][patch][999582] = http://drupal.org/files/issues/999582-1_notices.patch

; Creates invalid HTML with preformatted blocks.
; http://drupal.org/node/684554
projects[markdown][patch][684554] = http://drupal.org/files/issues/markdown-684554.patch

; -------------
; Libraries
; -------------

libraries[html5bp][download][type] = "file"
libraries[html5bp][download][url] = "http://github.com/paulirish/html5-boilerplate/zipball/v1.0stripped"
libraries[jquery][download][type] = "file"
libraries[jquery][download][url] = "https://ajax.googleapis.com/ajax/libs/jquery/1.6.0/jquery.min.js"
libraries[jqueryui][download][type] = "file"
libraries[jqueryui][download][url] = "https://ajax.googleapis.com/ajax/libs/jqueryui/1.8.12/jquery-ui.min.js"
libraries[jsonpath][download][url] = http://jsonpath.googlecode.com/files/jsonpath-0.8.1.php
libraries[jsonpath][download][type] = file
libraries[jsonpath][directory_name] = jsonpath
libraries[jsonpath][destination] = modules/feeds_jsonpath_parser
libraries[facebook-php-sdk][download][type] = "git"
libraries[facebook-php-sdk][download][url] = "git://github.com/facebook/php-sdk.git"
libraries[facebook-php-sdk][directory_name] = facebook-php-sdk
