; -------------
; Kendra Social
; drush makefile
; www.kendra.org
; -------------
; This file describes the module, theme, and patch requirements for Kendra Social 7.x.
; Add any modules, themes, libraries, and patches here to be used by an installation of Kendra Social.
; They will be inherited by other profiles which are based on Kendra Social.
; -------------
; Core version
; -------------

api = 2
core = 7.x
projects[drupal][type] = core
projects[drupal][version] = "7.0"

; -------------
; Modules
; -------------

; Kendra API
projects[kendra_api][subdir] = features
projects[kendra_api][type] = module
projects[kendra_api][download][type] = git
projects[kendra_api][download][url] = https://github.com/kendrainitiative/kendra_api.git

; Kendra Match
projects[kendra_match][subdir] = features
projects[kendra_match][type] = module
projects[kendra_match][download][type] = git
projects[kendra_match][download][url] = https://github.com/kendrainitiative/kendra_match.git

; contrib
;projects[admin][subdir] = contrib
;projects[admin][version] = 2.0-beta3
projects[admin_menu][subdir] = contrib
projects[admin_menu][version] = 3.0-rc1
projects[boxes][subdir] = contrib
projects[boxes][version] = 1.0-beta5
projects[context][subdir] = contrib
projects[context][version] = 3.0-beta1
projects[ctools][subdir] = contrib
projects[ctools][version] = 1.0-rc1
projects[devel][subdir] = contrib
projects[devel][version] = 1.2
;projects[diff][subdir] = contrib
;projects[diff][version] = 2.0-beta2
projects[entity][subdir] = contrib
projects[entity][version] = 1.0-beta10
projects[fb][subdir] = contrib
projects[fb][version] = 3.x-dev
projects[features][subdir] = contrib
projects[features][version] = 1.0-beta3
projects[feeds][subdir] = contrib
projects[feeds][version] = 2.0-alpha4
projects[fivestar][subdir] = contrib
projects[fivestar][version] = 2.0-alpha2
projects[votingapi][subdir] = contrib
projects[votingapi][version] = 2.6
projects[community_tags][subdir] = contrib
projects[community_tags][version] = 1.0
projects[html5_base][subdir] = contrib
projects[html5_base][version] = 2.x-dev
projects[job_scheduler][subdir] = contrib
projects[job_scheduler][version] = 2.0-alpha2
projects[jquery_plugin][subdir] = contrib
projects[jquery_plugin][version] = 1.0
projects[jquery_update][subdir] = contrib
projects[jquery_update][version] = 2.2
projects[libraries][subdir] = contrib
projects[libraries][version] = 1.0
projects[markdown][subdir] = contrib
projects[markdown][version] = 1.0-beta1
projects[messaging][subdir] = contrib
projects[messaging][version] = 1.0-alpha1
;projects[notifications][subdir] = contrib
;projects[notifications][version] = 1.0-alpha1
projects[openidadmin][subdir] = contrib
projects[openidadmin][version] = 1.0
projects[pathauto][subdir] = contrib
projects[pathauto][version] = 1.0-rc2
projects[rules][subdir] = contrib
projects[rules][version] = 2.0-beta3
projects[strongarm][subdir] = contrib
projects[strongarm][version] = 2.0-beta2
projects[token][subdir] = contrib
projects[token][version] = 1.0-beta6
projects[views][subdir] = contrib
projects[views][version] = 3.0-rc1
projects[oauth][subdir] = contrib 
projects[oauth][version] = 3.0-alpha1

; services
projects[services][subdir] = contrib 
projects[services][version] = 3.x-dev
;projects[services][download][type] = git
;projects[services][download][url] = http://git.drupal.org/project/services.git
;projects[services][download][url] = https://github.com/kendrainitiative/drupal_services.git
;projects[services][download][branch] = 7.x-3.x 

projects[rdfx][subdir] = contrib
projects[sparql][subdir] = contrib

; feeds_jsonpath_parser
;projects[feeds_jsonpath_parser][patch][] = http://drupal.org/files/feeds_jsonpath_parser-drush_make.patch
;projects[feeds_jsonpath_parser][version] = 1.x-dev
projects[feeds_jsonpath_parser][subdir] = contrib
projects[feeds_jsonpath_parser][type] = module
projects[feeds_jsonpath_parser][download][type] = file
projects[feeds_jsonpath_parser][download][url] = http://www.kendra.org.uk/feeds_jsonpath_parser-patched.tgz

; -------------
; Themes
; -------------

projects[tao][subdir] = contrib
projects[tao][version] = 3.0-beta3

projects[rubik][subdir] = contrib
projects[rubik][version] = 4.0-beta4

; -------------
; Patches
; -------------

; Notices on admin/index
; http://drupal.org/node/999582
projects[rubik][patch][999582] = http://drupal.org/files/issues/999582-1_notices.patch

; Creates invalid HTML with preformatted blocks.
; http://drupal.org/node/684554
projects[markdown][patch][684554] = http://drupal.org/files/issues/markdown-684554.patch

;projects[services][patch][] = "https://raw.github.com/kendrainitiative/kendra_social/master/patches/services-resource-access-defaults.patch"
projects[services][patch][1355952][url] = "https://raw.github.com/kendrainitiative/kendra_social/master/patches/services-libraries-spyc.patch"

; -------------
; Libraries
; -------------

; html5-boilerplate
libraries[html5bp][download][type] = "file"
libraries[html5bp][download][url] = "http://github.com/paulirish/html5-boilerplate/zipball/v1.0stripped"

; jquery + jqueryui
libraries[jquery][download][type] = "file"
libraries[jquery][download][url] = "https://ajax.googleapis.com/ajax/libs/jquery/1.6.0/jquery.min.js"
libraries[jqueryui][download][type] = "file"
libraries[jqueryui][download][url] = "https://ajax.googleapis.com/ajax/libs/jqueryui/1.8.12/jquery-ui.min.js"

; jsonpath
libraries[jsonpath][download][url] = http://jsonpath.googlecode.com/files/jsonpath-0.8.1.php
libraries[jsonpath][download][type] = file
libraries[jsonpath][directory_name] = jsonpath
libraries[jsonpath][destination] = modules/contrib/feeds_jsonpath_parser

; Facebook PHP SDK
libraries[facebook-php-sdk][download][type] = "git"
libraries[facebook-php-sdk][download][url] = "git://github.com/facebook/php-sdk.git"
libraries[facebook-php-sdk][directory_name] = facebook-php-sdk

; Spyc: required for Services 3.1
libraries[spyc][download][url] = http://spyc.googlecode.com/svn/trunk/spyc.php
libraries[spyc][download][type] = file
;libraries[spyc][directory_name] = spyc
libraries[spyc][destination] = libraries

; custom libraries for Kendra Social
libraries[kendra_social_test][download][[type] = file
libraries[kendra_social_test][download][[url] = https://raw.github.com/jamespadolsey/prettyPrint.js/master/prettyprint.js

; ARC2 libraries for SPARQL
libraries[arc][download][type]= "git"
libraries[arc][download][url] = "https://github.com/semsol/arc2.git"
libraries[arc][directory_name] = "arc"
libraries[arc][destination] = "libraries/ARC2"
