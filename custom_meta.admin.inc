<?php
/**
 * @file
 * Admin form to add custom meta tags
 * custom_meta_form. Hans.Gutknecht@mlssoccer.com, Louis.Jimenez@mlssoccer.com
 *
 */
function custom_meta_form() {
  $form = array();
  $form['description'] = array(
    '#prefix' => '<div>',
    '#value' => 'The Custom Meta module provides an interface for adding meta tags. This will add the created meta tags to the html head of all non-admin pages. Please be careful with the http-equiv meta attribute, these are added to <strong>ALL PAGES</strong>, so you could very easily break your site with redirects, expires, etc.<br /><br />Formatting for tags <em>&lt;meta [Meta Attribute]="[Attribute Value]" content="[Content value]"&gt;</em>',
    '#suffix' => '</div>',
  );
  $form['custom_meta_attribute'] = array(
      '#title' => t('Meta attribute'),
      '#type' => 'select',
      '#default_value' => variable_get('custom_meta_attribute', 'property'),
      '#options' => array('property' => 'property', 'name' => 'name', 'http-equiv' => 'http-equiv')
  );
  $form['custom_meta_attribute_value'] = array(
      '#title' => t('Attribute value'),
      '#type' => 'textfield',
      '#description' => t('Value for the attribute defined above'),
      '#default_value' => variable_get('custom_meta_attribute_value', '')
  );
  $form['custom_meta_content_value'] = array(
      '#title' => t('Content value'),
      '#type' => 'textfield',
      '#description' => t('Value for the meta content defined above'),
      '#default_value' => variable_get('custom_meta_content_value', '')
  );
  $form['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Add tag'),
  );
  $form['custom_meta_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of tags'),
  );

  $meta_tags = custom_meta_get_tags(NULL);

  if ($meta_tags) {
    foreach ($meta_tags as $key => $meta_tag) {
      $form['custom_meta_tags']['meta_tag_' . $key] = array(
        '#prefix' => '<div>',
        '#value' => '&lt;meta ' . $meta_tag['cm_meta_attr'] . '="' . $meta_tag['cm_meta_attr_value'] . '" content="' . $meta_tag['cm_meta_content'] . '"&gt; <div class="actions"><a href="custom_meta/edit/' . $meta_tag['cm_meta_uid'] . '">Edit</a> | <a href="custom_meta/remove/'. $meta_tag['cm_meta_uid'] . '">Remove</a>',
        '#suffix' => '</div>',
      );
    }
  }
  return $form;
}

function custom_meta_edit_form() {
  $form = array();
  $id = arg(4);
  $tag = custom_meta_get_tags($id);
  $form['custom_meta_attribute'] = array(
    '#title' => t('Meta attribute'),
    '#type' => 'select',
    '#description' => t('Either property, name or http-equiv'),
    '#default_value' => 'wtf',
    '#options' => array('property' => 'property', 'name' => 'name', 'http-equiv' => 'http-equiv')
  );
  $form['custom_meta_attribute_value'] = array(
    '#title' => t('Meta attribute value'),
    '#type' => 'textfield',
    '#description' => t('Value for the meta attribute defined above'),
    '#default_value' => $tag['cm_meta_attr_value']
  );
  $form['custom_meta_content_value'] = array(
    '#title' => t('Meta content value'),
    '#type' => 'textfield',
    '#description' => t('Value for the meta content defined above'),
    '#default_value' => $tag['cm_meta_content']
  );

  $form['submit'] = array(
   '#type' => 'submit',
   '#value' => t('Save tag'),
  );

  $form['cancel'] = array(
   '#value' => '<a href="/admin/settings/custom_meta">' . t('Cancel') . '</a>',
  );

  return $form;
}

/*
 * Form validation for main form.
 *
 */
function custom_meta_form_validate($form, &$form_state) {
  $atvalue = $form_state['values']['custom_meta_attribute_value'];
  $metacontent = $form_state['values']['custom_meta_content_value'];
  if (empty($atvalue)) {
    form_set_error('custom_meta_attribute_value', t('Must provide a value for meta attribute'));
  }
  if (empty($metacontent)) {
    form_set_error('custom_meta_content_value', t('Must provide a value for content'));
  }
}

/**
 * Submission function for custom_meta_form.
 */
function custom_meta_form_submit(&$form, &$form_state) {
  drupal_set_message(t('All your tag belong to us. Tag saved.'));

  custom_meta_set_tag($form_state['values']['custom_meta_attribute'], $form_state['values']['custom_meta_attribute_value'], $form_state['values']['custom_meta_content_value']);
}

/**
 * Submission function for custom_meta_edit_form.
 */
function custom_meta_edit_form_submit(&$form, &$form_state) {
  db_query("UPDATE {custom_meta} SET cm_meta_attr = '%s', cm_meta_attr_value = '%s', cm_meta_content = '%s' WHERE cm_meta_uid = '%d'", $form_state['clicked_button']['#post']['custom_meta_attribute'], $form_state['clicked_button']['#post']['custom_meta_attribute_value'], $form_state['clicked_button']['#post']['custom_meta_content_value'], arg(4));
  drupal_goto('admin/settings/custom_meta');
  drupal_set_message(t('Tag saved.'));
}
