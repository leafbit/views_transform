<?php

/**
 * @file
 * Definition of views_transform_field_handler_form_field.
 */

/**
 * Provides a handler that adds form and form field(s).
 */

class views_transform_field_handler_form_field extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['default_value_code'] = array('default' => '');
    $options['submit_php_code'] = array('default' => '');
    $options['form_element_type'] = array('default' => 'textfield');
    $options['form_element_size'] = array('default' => 10);
    $options['form_alter_code'] = array('default' => '');
    return $options;
  }

  /**
   * @inheritdoc
   */
  function options_form(&$form, &$form_state) {

    $parent_form = $form;
    parent::options_form($parent_form, $form_state);
    //  dpm($parent_form);
    //dpm($this->options);
    $form['custom_label'] = $parent_form['custom_label'];
    $form['label'] = $parent_form['label'];
    $form['element_label_colon'] = $parent_form['element_label_colon'];
    $form['exclude'] = $parent_form['exclude'];

    $suported_types = array(
      'checkbox' => 'checkbox',
      'textfield' => 'textfield',
    );

    $form['form_element_type'] = array(
      '#type' => 'select',
      '#title' => t('Form element type'),
      '#default_value' => $this->options['form_element_type'],
      '#options' => $suported_types,
    );

    $form['form_element_size'] = array(
      '#title' => t('Field size'),
      '#type' => 'textfield',
      '#size' => 12,
      '#required' => FALSE,
      '#default_value' => $this->options['form_element_size'],
      '#description' => t('The size of the field'),
    );

    $all_fields = $this->view->display_handler->get_field_labels();
    $form['default_value_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Default value'),
      // '#options' => $all_fields,
      '#default_value' => $this->options['default_value_code'],
      '#description' => 'The values $this, $row are available.'
        . ' Do not include &lt;' . '?php ?' . '&gt; tags.'
        . ' Retun the value for field default.',
    );

    $form['submit_php_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Form submit code'),
      '#default_value' => $this->options['submit_php_code'],
      '#description' => t('The code to be executed at form submission. Do not include &lt;' . '?php ?' . '&gt; tags') . '. Variables $form, $form_state available.' . ' Note also $this->row2val($row_index, $field_key=\'id\') function can help to retrieve values from view result.',
    );

    $form['form_alter_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Form alter code'),
      '#default_value' => $this->options['form_alter_code'],
      '#description' => 'Form alter code &$form, &$form_state available. Do not include &lt;' . '?php ?' . '&gt; tags',
    );
  }

  /**
   * @inheritdoc
   */
  function render($values) {
    // Render a Views form item placeholder.
    // This causes Views to wrap the View in a form.
    // Render a Views form item placeholder.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  function row2val($row_index, $idkey = 'id') {
    if (!isset($this->view->result[$row_index])) return FALSE;
    $row = &$this->view->result[$row_index];
    if (!isset($row->$idkey)) return FALSE;
    return $row->$idkey;
  }

  /**
   * Add to and alter the form created by Views.
   */
  function views_form(&$form, &$form_state) {
    // Create a container for our replacements
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    $form['helper_hidden'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
    if ($this->options['form_alter_code']) {
      $form['#views_transform_php'][] = $this->options['form_alter_code'];
    }
    // Iterate over the result and add our replacement fields to the form.
    foreach($this->view->result as $row_index => $row) {
      // Add a text field to the form.  This array convention
      // corresponds to the placeholder HTML comment syntax.
      $dv = $this->options['default_value_code'];
      if ($dv) $dv = eval($dv);
      $form[$this->options['id']][$row_index] = array(
        '#type' => $this->options['form_element_type'],
        '#default_value' => $dv,
        '#element_validate' => array('views_transform_field_handler_form_field_validate'),
      );
      $fld = &$form[$this->options['id']][$row_index];
      $size = intval($this->options['form_element_size']);
      if ($size) $fld['#size'] = $size;
      unset($fld);
    }
  }

   /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    eval($this->options['submit_php_code']);
  }

  /**
   * @inheritdoc
   */
  function query() { }
}

/**
 * Function views_transform_field_handler_form_field_validate().
 */
function views_transform_field_handler_form_field_validate($element, &$form_state) {
  // Handle errors?
}
?>
