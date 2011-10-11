<?php

function kendra_social_install_tasks($install_state) {
  $tasks = array();
  $tasks['configure_kendra_social'] = array(
    'display_name' => st('Configure Kendra Social'),
    'display' => TRUE,
    'type' => 'normal',
    'run' => INSTALL_TASK_RUN_IF_REACHED,
    'function' => 'kendra_social_install_task_configure',
  );
}

function kendra_social_install_task_configure() {
  return NULL;
}

