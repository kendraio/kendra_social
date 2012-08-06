; -------------
; Kendra Social
; drush makefile
; www.kendra.org
; -------------
; This file describes the core project requirements for Kendra Social 7.x. Several
; patches against Drupal core and their associated issue numbers have been
; included here for reference.
;
; Use this file to build a full distro including Drupal core (with patches) and
; the Kendra Social install profile using the following command:
;
;     $ drush make distro.make [directory]
;
; -------------
; Core version
; -------------

api = 2
core = 7.x
projects[drupal][type] = core
projects[drupal][version] = "7.15"

; -------------
; Patches
; -------------

; Missing drupal_alter() for text formats and filters
; http://drupal.org/node/903730
projects[drupal][patch][903730] = http://drupal.org/files/issues/drupal.filter-alter.92.patch

; Use vocabulary machine name for permissions
; http://drupal.org/node/995156
projects[drupal][patch][995156] = http://drupal.org/files/995156-31_portable_taxonomy_permissions-D7-15.patch

; -------------
; Profile
; -------------

projects[kendra_social][type] = profile
projects[kendra_social][download][type] = git
projects[kendra_social][download][url] = git://github.com/kendrainitiative/kendra_social.git
;projects[kendra_social][download][tag] = 7.x-1.0-dev
