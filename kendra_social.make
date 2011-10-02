; -------------
; Kendra Social
; drush makefile
; www.kendra.org
; -------------
; Core version
; -------------

core = 7.x
  
; -------------
; API version
; -------------

api = 2
  
; -------------
; Core project
; -------------

projects[drupal][version] = 7
projects[drupal][type] = core

; -------------
; Modules
; -------------

projects[admin][version] = 2.0-beta3
projects[admin_menu][version] = 3.0-rc1
projects[ctools][version] = 1.0-rc1
projects[devel][version] = 1.2
projects[features][version] = 1.0-beta3
projects[fivestar][version] = 2.x-dev
projects[html5_base][version] = 2.x-dev
projects[libraries][version] = 1.0
projects[modernizr][version] = 1.0
projects[notifications][version] = 1.0-alpha1
projects[pathauto][version] = 1.0-rc2
projects[strongarm][version] = 2.0-beta2
projects[rules][version] = 2.0-beta3
projects[jquery_plugin][version] = 1.0
projects[jquery_update][version] = 2.2
projects[views][version] = 3.0-rc1
projects[job_scheduler][version] = 2.0-alpha2
projects[feeds][version] = 2.0-alpha4
projects[feeds_jsonpath_parser][version] = 1.0-beta2
projects[] = openidadmin
projects[] = oauth
projects[] = services

; -------------
; Themes
; -------------

projects[tao][version] = 3.0-beta3

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
libraries[jsonpath][destination] = profiles/kendra_social/modules/feeds_jsonpath_parser/jsonpath-0.8.1.php
