<?php
/*
 * @file
 * Protected node mail handling.
 */


/**
 * Protected node sends email to specified users.
 *
 * @todo
 * We should properly handle the language...
 */
function protected_node_send_mail($node) {
  module_load_include('settings.inc', 'protected_node');

  $body = variable_get('protected_node_email_body', protected_node_email_body());
  if (strpos('[node-password]', $body) !== FALSE) {
    if (empty($node->protected_node_clear_passwd)) {
      drupal_set_message(t('The protected node email was not sent because the [node-password] is undefined. You must re-enter the password before saving in order to send the password.'), 'error');
      return;
    }
  }

  $from = variable_get('protected_node_email_from', '');
  if (!$from) {
    // needs to be null to work properly inside drupal_mail()
    $from = NULL;
  }
  drupal_mail('protected_node', 'password',
              $node->protected_node_emails, 'en',
              array('node' => $node), $from, TRUE);

  drupal_set_message(t('The email about this page was sent to the specified email addresses.'));
}

/**
 * Implementation of hook_mail().
 *
 * This is the function that builds the actual message body.
 */
function protected_node_mail($key, &$message, $params) {
  global $user;

  $has_token = module_exists('token');
  $node = $params['node'];

  switch ($key) {
  case 'password':
    module_load_include('settings.inc', 'protected_node');

    $subject = variable_get('protected_node_email_subject', protected_node_email_subject());
    if ($has_token) {
      $subject = token_replace($subject, 'node', $node);
      $subject = token_replace($subject, 'user', $user);
    }
    $message['subject'] = $subject;

    $body = variable_get('protected_node_email_body', protected_node_email_body());
    if ($has_token) {
      $body = token_replace($body, 'node', $node);
      $body = token_replace($body, 'user', $user);
    }
    $message['body'] = $body;
    break;

  }
}

// vim: ts=2 sw=2 et syntax=php
