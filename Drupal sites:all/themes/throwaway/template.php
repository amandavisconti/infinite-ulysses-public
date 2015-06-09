<?php 
// Amanda
/** 
 * Primary pre/preprocess functions and alterations.
 */ 

/* Let my theme override page.tpl.php's for specific node types */
function throwaway_preprocess_page(&$variables, $hook) {
  if (isset($variables['node'])) {
    $variables['theme_hook_suggestions'][] = 'page__node_' . $variables['node']->type;    
  }
}

/* Add hover info to user pictures */
function throwaway_preprocess_user_picture(&$variables) {
  $variables['user_picture'] = '';
  if (variable_get('user_pictures', 0)) {
    $account = $variables['account'];
    if (!empty($account->picture)) {
      // @TODO: Ideally this function would only be passed file objects, but
      // since there's a lot of legacy code that JOINs the {users} table to
      // {node} or {comments} and passes the results into this function if we
      // a numeric value in the picture field we'll assume it's a file id
      // and load it for them. Once we've got user_load_multiple() and
      // comment_load_multiple() functions the user module will be able to load
      // the picture files in mass during the object's load process.
      if (is_numeric($account->picture)) {
        $account->picture = file_load($account->picture);
      }
      if (!empty($account->picture->uri)) {
        $filepath = $account->picture->uri;
      }
    }
    elseif (variable_get('user_picture_default', '')) {
      $filepath = variable_get('user_picture_default', '');
    }
    if (isset($filepath)) {
      $alt = t("Annotation author: @user", array('@user' => format_username($account)));
      // If the image does not have a valid Drupal scheme (for eg. HTTP),
      // don't load image styles.
      if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
        $variables['user_picture'] = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt));
      }
      else {
        $variables['user_picture'] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt));
      }
      if (!empty($account->uid) && user_access('access user profiles')) {
        $attributes = array('attributes' => array('title' => t('View user profile.')), 'html' => TRUE);
        $variables['user_picture'] = l($variables['user_picture'], "user/$account->uid", $attributes);
      }
    }
  }
} 

/* Bookmark flags as images instead of text */
function throwaway_preprocess_flag(&$vars) {
    $image_file = $vars['directory'] . '/img/icons/toggle-' . ($vars['action'] == 'flag' ? 'off' : 'on') . '.png';
    if (file_exists($image_file)) {
      $image = theme_image(array('path' => $image_file, 'attributes' => array('class' => array('MY-CLASS'))));
      $vars['link_text'] = $image;
    }
}