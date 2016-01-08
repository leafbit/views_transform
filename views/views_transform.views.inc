<?php

/**
 * Invoke hook_views_data_alter().
 */
function views_transform_views_data() {
  //dpm($data);
  // Add the Title form field to
  $data['views']['views_transform'] = array(
    'field' => array(
      'title' => t('Form field'),
      'help' => t('Appends a form field and transforms view into a multifield form'),
      'handler' => 'views_transform_field_handler_form_field',
    ),
  );

  return $data;
}

/**
 * Invoke hook_views_query_substitutions().
 */
function views_transform_views_query_substitutions($view) {
  $s = array();
  $parts = explode('/' , current_path());
  foreach ($parts as $k => $v) {
    $s['***ARG' . $k .'***'] = $v;
  }
  if (isset($view->args)) {
    foreach($view->args as $k => $v) {
      $s['***VIEW_ARG' . $k .'***'] = $v;
    }
  }
  global $user;
  $s['***URL_USER_OR_CURRENT***'] = preg_match("/^user\/(\d+)/", current_path(), $mt) ? $mt[1] : $user->uid;
  return $s;
}
?>
