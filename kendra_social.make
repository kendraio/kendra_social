; -------------
; Kendra Social
; drush makefile
; www.kendra.org
; -------------
; This file describes the module, theme, and patch requirements for Kendra Social 7.x.
; Add any modules, themes, libraries, and patches here to be used by an installation of Kendra Social.
; They will NOT be inherited by other profiles which are based on Kendra Social.
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

; -------------
; Themes
; -------------

; -------------
; Patches
; -------------

; -------------
; Libraries
; -------------
