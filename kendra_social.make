api = 2
core = 7.x

; Include Build Kit distro makefile via URL
;includes[] = http://drupalcode.org/project/buildkit.git/blob_plain/refs/heads/7.x-2.x:/distro.make

projects[drupal][type] = core

; Add kendra_social to the full Drupal distro build
projects[kendra_social][type] = profile
projects[kendra_social][download][type] = git
projects[kendra_social][download][url] = git://github.com/kendrainitiative/kendra_social.git

;projects[ctools][version] = "1.0-rc1"
;projects[ctools][subdir] = contrib

;projects[features][version] = "1.x-dev"
;projects[features][subdir] = dev

projects[libraries][version] = "2.x-dev"
projects[libraries][subdir] = contrib

projects[oauth][version] = "3.0-alpha1"
projects[oauth][subdir] = contrib

;projects[views][version] = "3.0-rc1"
;projects[views][subdir] = contrib
