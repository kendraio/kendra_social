api = 2
core = 7.x

; Include Build Kit distro makefile via URL
includes[] = http://drupalcode.org/project/buildkit.git/blob_plain/refs/heads/7.x-2.x:/distro.make

; Add kendra_social to the full Drupal distro build
projects[kendra_social][type] = profile
projects[kendra_social][download][type] = git
projects[kendra_social][download][url] = git://github.com/kendrainitiative/kendra_social.git
