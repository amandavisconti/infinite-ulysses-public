<?php

/**
 * @file
 * Mmenu administration menu items.
 */

/**
 * Callback function for the Mmenu admin page.
 */
function mmenu_admin_settings($mmenu_name = '') {
  if (!empty($mmenu_name)) {
    drupal_goto('admin/config/mmenu/' . $mmenu_name);
  }
  return '';
}

/**
 * Form constructor for the Mmenu settings form.
 */
function mmenu_admin_settings_form($form, &$form_state, $mmenu_name = '') {
  $mmenu = mmenu_list($mmenu_name);
  $site_name = variable_get('site_name', t('Drupal'));
  $bool_options = array(
    'true' => t('Yes'),
    'false' => t('No'),
  );

  $form['general'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#weight' => -5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general']['enabled'] = array(
    '#title' => t('Enabled?'),
    '#type' => 'select',
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
    '#default_value' => $mmenu['enabled'] ? 1 : 0,
    '#required' => TRUE,
    '#weight' => -3,
    '#description' => t('Enable or disable the mmenu.'),
  );
  $form['general']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $mmenu['title'],
    '#required' => TRUE,
    '#weight' => -1,
    '#description' => t('The administrator title of the mmenu.'),
  );

  $form['general']['name'] = array(
    '#type' => 'hidden',
    '#value' => $mmenu_name,
  );

  $drupal_blocks = mmenu_get_blocks();
  $options = array();
  $options[] = t('--- Please select a block ---');
  foreach ($drupal_blocks as $module => $drupal_block) {
    foreach ($drupal_block as $delta => $info) {
      $options[$module . '|' . $delta] = drupal_ucfirst($module) . ' - ' . $info['info'];
    }
  }

  $form['blocks'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Blocks'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $blocks = array();
  foreach ($mmenu['blocks'] as $k => $block) {
    $blocks[] = $block;
  }
  $mmenu_allowed_blocks_nums = variable_get('mmenu_allowed_blocks_nums', 5);
  for ($i = count($blocks); $i < $mmenu_allowed_blocks_nums; $i++) {
    $blocks[$i]['title'] = '';
    $blocks[$i]['module'] = '';
    $blocks[$i]['delta'] = '';
    $blocks[$i]['collapsed'] = TRUE;
    $blocks[$i]['wrap'] = FALSE;
  }

  foreach ($blocks as $k => $block) {
    $form['blocks'][$k] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Block'),
      '#collapsible' => TRUE,
      '#collapsed' => !empty($block['module']) && !empty($block['delta']) ? FALSE : TRUE,
    );
    $form['blocks'][$k]['module_delta'] = array(
      '#title' => t('Select a block'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => !empty($block['module']) && !empty($block['delta']) ? $block['module'] . '|' . $block['delta'] : '',
      '#description' => t('Select a block to display on the mmenu.'),
    );
    $form['blocks'][$k]['menu_parameters'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Menu parameters'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['blocks'][$k]['menu_parameters']['min_depth'] = array(
      '#title' => t('Min depth'),
      '#type' => 'select',
      '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9)),
      '#default_value' => (isset($block['menu_parameters']) && isset($block['menu_parameters']['min_depth'])) ? $block['menu_parameters']['min_depth'] : 1,
      '#description' => t('The minimum depth of menu links in the resulting tree. Defaults to 1, which is the default to build a whole tree for a menu (excluding menu container itself).'),
    );

    $form['blocks'][$k]['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => $block['title'],
      '#description' => t('Override the default title for the block. Use <em>!placeholder</em> to display no title, or leave blank to use the default block title.', array('!placeholder' => '&lt;none&gt;')),
    );
    $form['blocks'][$k]['collapsed'] = array(
      '#title' => t('Collapsed'),
      '#type' => 'select',
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
      '#default_value' => $block['collapsed'] ? 1 : 0,
      '#description' => t('Collapse or expand the block content by default.'),
    );
    $form['blocks'][$k]['wrap'] = array(
      '#title' => t('Wrap'),
      '#type' => 'select',
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
      ),
      '#default_value' => $block['wrap'] ? 1 : 0,
      '#description' => t('Determine if needs to wrap the block content. Usually to set it to true if the block is not a system menu.'),
    );
  }

  $options = $mmenu['options'];
  $form['options'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Mmenu options'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/options')),
  );

  $classes = mmenu_theme_list();
  $class_options = array();
  foreach ($classes as $class) {
    $class_options[$class['name']] = $class['title'];
  }
  $form['options']['classes'] = array(
    '#title' => t('theme'),
    '#type' => 'select',
    '#options' => $class_options,
    '#default_value' => $options['classes'],
    '#description' => t('A collection of space-separated classnames to add to both the menu and the HTML.'),
  );

  $effects = mmenu_effect_list();
  $effect_options = array();
  foreach ($effects as $effect) {
    $effect_options[$effect['name']] = $effect['name'] . ': ' . $effect['title'];
  }
  $form['options']['effects'] = array(
    '#title' => t('effects'),
    '#type' => 'checkboxes',
    '#options' => $effect_options,
    '#default_value' => isset($options['effects']) ? $options['effects'] : array(),
    '#description' => t('To apply additional effects on the page, the menu and the panels.'),
  );
  $form['options']['slidingSubmenus'] = array(
    '#title' => t('slidingSubmenus'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['slidingSubmenus']) && $options['slidingSubmenus'] ? 'true' : 'false',
    '#description' => t('Whether or not the submenus should come sliding in from the right. If false, the submenus expand below their parent.'),
  );

  $form['options']['buttonbars'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('buttonbars (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/buttonbars.html')),
  );
  $form['options']['buttonbars']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no options.'),
  );

  $form['options']['clickOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('clickOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['options']['clickOpen']['open'] = array(
    '#title' => t('open'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['clickOpen']['open']) && $options['clickOpen']['open'] ? 'true' : 'false',
    '#description' => t('Whether or not the menu should open when clicking on the selector.'),
  );
  $form['options']['clickOpen']['selector'] = array(
    '#title' => t('selector'),
    '#type' => 'textfield',
    '#default_value' => isset($options['clickOpen']['selector']) ? $options['clickOpen']['selector'] : '',
    '#description' => t("Clicks the selector to open the mmenu. e.g. #logo or a[id=logo]"),
  );

  $form['options']['counters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('counters (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/counters.html')),
  );
  $form['options']['counters']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['counters']['add']) && $options['counters']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically append a counter to each menu item that has a submenu.'),
  );
  $form['options']['counters']['update'] = array(
    '#title' => t('update'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['counters']['update']) && $options['counters']['update'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically count the number of items in the submenu, updates when typing in the search field.'),
  );

  $form['options']['dragOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dragOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/drag-open.html')),
  );
  $form['options']['dragOpen']['open'] = array(
    '#title' => t('open'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['dragOpen']['open']) && $options['dragOpen']['open'] ? 'true' : 'false',
    '#description' => t('Whether or not the menu should open when dragging the page.') . '<br />' . t('Tip: The jQuery.hammer plugin enables dragging on touch and desktop devices. If you only want to enable dragging the menu open on touchscreens, test for touch support using $.fn.mmenu.support.touch.'),
  );
  $form['options']['dragOpen']['pageNode'] = array(
    '#title' => t('pageNode'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dragOpen']['pageNode']) ? $options['dragOpen']['pageNode'] : 'body',
    '#description' => t('The node on which the user can drag to open the menu. If omitted, the entire page is used.'),
  );
  $form['options']['dragOpen']['threshold'] = array(
    '#title' => t('threshold'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dragOpen']['threshold']) ? $options['dragOpen']['threshold'] : 100,
    '#description' => t('The minimum amount of pixels to drag before actually opening the menu, less than 50 is not advised.'),
  );
  $form['options']['dragOpen']['maxStartPos'] = array(
    '#title' => t('maxStartPos'),
    '#type' => 'textfield',
    '#default_value' => isset($options['dragOpen']['maxStartPos']) ? $options['dragOpen']['maxStartPos'] : 50,
    '#description' => t('The maximum x-position to start dragging the page.For a menu with a position set to "top" or "bottom", the default value is 50.'),
  );

  $form['options']['fixedElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('fixedElements (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/fixed-elements.html')),
  );
  $form['options']['fixedElements']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no options.'),
  );

  $form['options']['footer'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('footer (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/footer.html')),
  );
  $form['options']['footer']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['footer']['add']) && $options['footer']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically append a fixed footer to the menu.'),
  );
  $form['options']['footer']['content'] = array(
    '#title' => t('content'),
    '#type' => 'textfield',
    '#default_value' => isset($options['footer']['content']) ? $options['footer']['content'] : '',
    '#description' => t('The contents of the footer. If omitted, the "title" option is used.'),
  );
  $form['options']['footer']['title'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['footer']['title']) ? $options['footer']['title'] : 'Copyright ©' . date('Y'),
    '#description' => t('The text of the footer if the submenu has no footer text.'),
  );
  $form['options']['footer']['update'] = array(
    '#title' => t('update'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['footer']['update']) && $options['footer']['update'] ? 'true' : 'false',
    '#description' => t('Whether or not to update the title with footer text from each submenu.'),
  );

  $form['options']['header'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('header (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/header.html')),
  );
  $form['options']['header']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['header']['add']) && $options['header']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically prepend a fixed header to the menu.'),
  );
  $form['options']['header']['content'] = array(
    '#title' => t('content'),
    '#type' => 'textfield',
    '#default_value' => isset($options['header']['content']) ? $options['header']['content'] : '',
    '#description' => t('The contents of the header. If omitted, the plugin will add a fully styled and functional header with a title, back- and next button.'),
  );
  $form['options']['header']['title'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['header']['title']) ? $options['header']['title'] : $site_name,
    '#description' => t('The text above the main menu.'),
  );
  $form['options']['header']['update'] = array(
    '#title' => t('update'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['header']['update']) && $options['header']['update'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically update the title, back- and next- button when opening submenus.'),
  );

  $form['options']['labels'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('labels (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/labels.html')),
  );
  $form['options']['labels']['collapse'] = array(
    '#title' => t('collapse'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['labels']['collapse']) && $options['labels']['collapse'] ? 'true' : 'false',
    '#description' => t('Whether or not to collapse all subsequent list-items that have the classname specified in classNames.labels.collapsed in the configuration object.'),
  );

  $form['options']['offCanvas'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offCanvas (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/off-canvas.html')),
  );
  $form['options']['offCanvas']['enabled'] = array(
    '#title' => t('flag'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['offCanvas']) && isset($options['offCanvas']['enabled']) && $options['offCanvas']['enabled'] ? 'true' : 'false',
    '#description' => t('Whether or not to enable the add-on.'),
  );
  $form['options']['offCanvas']['modal'] = array(
    '#title' => t('modal'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['offCanvas']['modal']) && $options['offCanvas']['modal'] ? 'true' : 'false',
    '#description' => t('Whether or not the menu should be opened as a "modal". Basically, this means the user has no default way of closing the menu. You\'ll have to provide a close-button yourself.'),
  );
  $form['options']['offCanvas']['moveBackground'] = array(
    '#title' => t('moveBackground'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['offCanvas']['moveBackground']) && $options['offCanvas']['moveBackground'] ? 'true' : 'false',
    '#description' => t('Whether or not the page should inherit the background of the body when the menu opens.'),
  );
  $form['options']['offCanvas']['position'] = array(
    '#title' => t('position'),
    '#type' => 'textfield',
    '#default_value' => $mmenu['position'],
    '#description' => t('The position of the menu relative to the page. Possible values: "top", "right", "bottom" or "left".'),
    '#attributes' => array(
      'readonly' => 'readonly',
    ),
  );
  $form['options']['offCanvas']['zposition'] = array(
    '#title' => t('zposition'),
    '#type' => 'textfield',
    '#default_value' => isset($options['offCanvas']['zposition']) ? $options['offCanvas']['zposition'] : 'front',
    '#description' => t('The z-position of the menu relative to the page. Possible values: "back", "front" or "next".'),
  );

  $form['options']['searchfield'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('searchfield (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/searchfield.html')),
  );
  $form['options']['searchfield']['add'] = array(
    '#title' => t('add'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['add']) && $options['searchfield']['add'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically prepend a search field to the menu.'),
  );
  $form['options']['searchfield']['addTo'] = array(
    '#title' => t('addTo'),
    '#type' => 'textfield',
    '#default_value' => isset($options['searchfield']['addTo']) ? $options['searchfield']['addTo'] : t('menu'),
    '#description' => t('Where to add the searchfield(s). Possible values: "menu", "panels" and a valid jQuery selector.'),
  );
  $form['options']['searchfield']['search'] = array(
    '#title' => t('search'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['search']) && $options['searchfield']['search'] ? 'true' : 'false',
    '#description' => t('Whether or not to automatically search when typing.'),
  );
  $form['options']['searchfield']['placeholder'] = array(
    '#title' => t('title'),
    '#type' => 'textfield',
    '#default_value' => isset($options['searchfield']['placeholder']) ? $options['searchfield']['placeholder'] : t('Search'),
    '#description' => t('The placeholder text in the search field.'),
  );
  $form['options']['searchfield']['noResults'] = array(
    '#title' => t('noResults'),
    '#type' => 'textfield',
    '#default_value' => isset($options['searchfield']['noResults']) ? $options['searchfield']['noResults'] : t('No results found.'),
    '#description' => t('The text to show when no results are found. If false no message will be shown.'),
  );
  $form['options']['searchfield']['showLinksOnly'] = array(
    '#title' => t('showLinksOnly'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($options['searchfield']['showLinksOnly']) && $options['searchfield']['showLinksOnly'] ? 'true' : 'false',
    '#description' => t('Whether or not to only show links (A elements) in the search results. If false, also SPAN elements will be shown.'),
  );

  $form['options']['toggles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('toggles (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/toggles.html')),
  );
  $form['options']['toggles']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no options.'),
  );

  $configurations = $mmenu['configurations'];
  $form['configurations'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Mmenu configurations'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('For more information about the configurations, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/options/configuration.html')),
  );
  $form['configurations']['clone'] = array(
    '#title' => t('clone'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($configurations['clone']) && $configurations['clone'] ? 'true' : 'false',
    '#description' => t('Whether or not to clone the menu before prepending it to the BODY. If true, the ID on the menu and every ID inside it will be prepended with "mm-" to prevent using double IDs.'),
  );
  $form['configurations']['preventTabbing'] = array(
    '#title' => t('preventTabbing'),
    '#type' => 'select',
    '#options' => $bool_options,
    '#default_value' => isset($configurations['preventTabbing']) && $configurations['preventTabbing'] ? 'true' : 'false',
    '#description' => t('Whether or not to prevent the default behavior when pressing the tab key. If false, the user will be able to tab out of the menu and using some other way to prevent this (for example TABguard) is strongly recommended.'),
  );
  $form['configurations']['panelNodetype'] = array(
    '#title' => t('panelNodetype'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['panelNodetype']) ? $configurations['panelNodetype'] : 'div, ul, ol',
    '#description' => t('The node-type of panels.'),
  );
  $form['configurations']['transitionDuration'] = array(
    '#title' => t('transitionDuration'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['transitionDuration']) ? $configurations['transitionDuration'] : 400,
    '#description' => t('The number of milliseconds used in the CSS transitions.'),
  );

  $form['configurations']['classNames'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('classNames'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['label'] = array(
    '#title' => t('label'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['label']) ? $configurations['classNames']['label'] : 'Label',
    '#description' => t('The classname on a LI that should be displayed as a label.'),
  );
  $form['configurations']['classNames']['panel'] = array(
    '#title' => t('panel'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['panel']) ? $configurations['classNames']['panel'] : 'Panel',
    '#description' => t('The classname on an element (for example a DIV) that should be considered to be a panel. Only applies if the "isMenu" option is set to false.'),
  );
  $form['configurations']['classNames']['selected'] = array(
    '#title' => t('selected'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['selected']) ? $configurations['classNames']['selected'] : 'Selected',
    '#description' => t('The classname on the LI that should be displayed as selected.'),
  );

  $form['configurations']['buttonbars'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('buttonbars (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/buttonbars.html')),
  );
  $form['configurations']['buttonbars']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options.'),
  );
  $form['configurations']['classNames']['buttonbars'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('buttonbars'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['buttonbars']['buttonbar'] = array(
    '#title' => t('buttonbar'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['buttonbars']['buttonbar']) ? $configurations['classNames']['buttonbars']['buttonbar'] : 'Buttonbar',
  );

  $form['configurations']['clickOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('clickOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['clickOpen']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options.'),
  );
  $form['configurations']['classNames']['clickOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('clickOpen'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['clickOpen']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no classNames options.'),
  );

  $form['configurations']['counters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('counters (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/counters.html')),
  );
  $form['configurations']['counters']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options.'),
  );
  $form['configurations']['classNames']['counters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('counters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['counters']['counter'] = array(
    '#title' => t('counter'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['counters']['counter']) ? $configurations['classNames']['counters']['counter'] : 'Counter',
  );

  $form['configurations']['dragOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dragOpen (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/drag-open.html')),
  );
  $form['configurations']['classNames']['dragOpen'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('dragOpen'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['dragOpen']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no classNames options.'),
  );
  $form['configurations']['dragOpen']['width'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('width'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dragOpen']['width']['perc'] = array(
    '#title' => t('perc'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['width']['perc']) ? $configurations['dragOpen']['width']['perc'] : 0.8,
    '#description' => t('The width of the menu as a percentage. From 0.0 (fully hidden) to 1.0 (fully opened).'),
  );
  $form['configurations']['dragOpen']['width']['min'] = array(
    '#title' => t('min'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['width']['min']) ? $configurations['dragOpen']['width']['min'] : 140,
    '#description' => t('The minimum width of the menu.'),
  );
  $form['configurations']['dragOpen']['width']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['width']['max']) ? $configurations['dragOpen']['width']['max'] : 440,
    '#description' => t('The maximum width of the menu.'),
  );
  $form['configurations']['dragOpen']['height'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('height'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['dragOpen']['height']['perc'] = array(
    '#title' => t('perc'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['height']['perc']) ? $configurations['dragOpen']['height']['perc'] : 0.8,
    '#description' => t('The height of the menu as a percentage. From 0.0 (fully hidden) to 1.0 (fully opened).'),
  );
  $form['configurations']['dragOpen']['height']['min'] = array(
    '#title' => t('min'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['height']['min']) ? $configurations['dragOpen']['height']['min'] : 140,
    '#description' => t('The minimum height of the menu.'),
  );
  $form['configurations']['dragOpen']['height']['max'] = array(
    '#title' => t('max'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['dragOpen']['height']['max']) ? $configurations['dragOpen']['height']['max'] : 440,
    '#description' => t('The maximum height of the menu.'),
  );

  $form['configurations']['fixedElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('fixedElements (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/counters.html')),
  );
  $form['configurations']['fixedElements']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['fixedElements'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('fixedElements'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['fixedElements']['fixedTop'] = array(
    '#title' => t('fixedTop'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['fixedElements']['fixedTop']) ? $configurations['classNames']['fixedElements']['fixedTop'] : 'header',
  );
  $form['configurations']['classNames']['fixedElements']['fixedBottom'] = array(
    '#title' => t('fixedTop'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['fixedElements']['fixedBottom']) ? $configurations['classNames']['fixedElements']['fixedBottom'] : 'footer',
  );

  $form['configurations']['footer'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('footer (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/footer.html')),
  );
  $form['configurations']['footer']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['footer'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('footer'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['footer']['panelFooter'] = array(
    '#title' => t('panelFooter'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['footer']['panelFooter']) ? $configurations['classNames']['footer']['panelFooter'] : 'Footer',
  );

  $form['configurations']['header'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('header (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/header.html')),
  );
  $form['configurations']['header']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['header'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('header'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['header']['panelHeader'] = array(
    '#title' => t('panelHeader'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['header']['panelHeader']) ? $configurations['classNames']['header']['panelHeader'] : 'Header',
  );
  $form['configurations']['classNames']['header']['panelNext'] = array(
    '#title' => t('panelNext'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['header']['panelNext']) ? $configurations['classNames']['header']['panelNext'] : 'Next',
  );
  $form['configurations']['classNames']['header']['panelPrev'] = array(
    '#title' => t('panelPrev'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['header']['panelPrev']) ? $configurations['classNames']['header']['panelPrev'] : 'Prev',
  );

  $form['configurations']['labels'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('labels (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/labels.html')),
  );
  $form['configurations']['labels']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['labels'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('labels'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['labels']['collapsed'] = array(
    '#title' => t('collapsed'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['labels']['collapsed']) ? $configurations['classNames']['labels']['collapsed'] : 'Collapsed',
  );

  $form['configurations']['offCanvas'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offCanvas (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/off-canvas.html')),
  );
  $form['configurations']['classNames']['offCanvas'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('offCanvas'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['offCanvas']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no classNames options.'),
  );
  $form['configurations']['offCanvas']['menuInjectMethod'] = array(
    '#title' => t('menuInjectMethod'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['menuInjectMethod']) ? $configurations['offCanvas']['menuInjectMethod'] : 'prepend',
    '#description' => t('How to inject the menu to the DOM. Possible values: "prepend" or "append".'),
  );
  $form['configurations']['offCanvas']['menuWrapperSelector'] = array(
    '#title' => t('menuWrapperSelector'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['menuWrapperSelector']) ? $configurations['offCanvas']['menuWrapperSelector'] : 'body',
    '#description' => t('jQuery selector for the node the menu should be injected in.'),
  );
  $form['configurations']['offCanvas']['pageNodetype'] = array(
    '#title' => t('pageNodetype'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['pageNodetype']) ? $configurations['offCanvas']['pageNodetype'] : 'div',
    '#description' => t('The node-type of the page.'),
  );
  $form['configurations']['offCanvas']['pageSelector'] = array(
    '#title' => t('pageSelector'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['offCanvas']['pageSelector']) ? $configurations['offCanvas']['pageSelector'] : 'body > div',
    '#description' => t('jQuery selector for the page. e.g. "body > " + offCanvas.pageNodetype'),
  );

  $form['configurations']['searchfield'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('searchfield (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/searchfield.html')),
  );
  $form['configurations']['classNames']['searchfield'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('searchfield'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['searchfield']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no classNames options.'),
  );
  $form['configurations']['searchfield']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options.'),
  );

  $form['configurations']['toggles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('toggles (add-on)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('For more information about the options, please visit the page <a href="!link">!link</a>.', array('!link' => 'http://mmenu.frebsite.nl/documentation/addons/toggles.html')),
  );
  $form['configurations']['toggles']['no_options'] = array(
    '#type' => 'markup',
    '#markup' => t('The add-on has no configuration options, it does add an object to the classNames option.'),
  );
  $form['configurations']['classNames']['toggles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('toggles'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['configurations']['classNames']['toggles']['toggle'] = array(
    '#title' => t('toggle'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['toggles']['toggle']) ? $configurations['classNames']['toggles']['toggle'] : 'Toggle',
  );
  $form['configurations']['classNames']['toggles']['check'] = array(
    '#title' => t('check'),
    '#type' => 'textfield',
    '#default_value' => isset($configurations['classNames']['toggles']['check']) ? $configurations['classNames']['toggles']['check'] : 'Check',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 0,
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
    '#weight' => 1,
  );

  return $form;
}

/**
 * Form submission handler for mmenu_admin_settings_form().
 */
function mmenu_admin_settings_form_submit($form_id, &$form_state) {
  $values = $form_state['values'];

  switch ($values['op']) {
    case t('Save'):
      $blocks = array();

      // Updates the blocks.
      foreach ($values['blocks'] as $k => $block) {
        if (!empty($block['module_delta'])) {
          $v = explode('|', $block['module_delta']);
          $blocks[$k] = $block;
          $blocks[$k] += array(
            'module' => $v[0],
            'delta' => $v[1],
          );
        }
      }

      // Updates the effects of options.
      if (isset($values['options']['effects'])) {
        foreach ($values['options']['effects'] as $k => $v) {
          if (!$v) {
            unset($values['options']['effects'][$k]);
          }
        }
      }

      $mmenu = array(
        'enabled' => $values['general']['enabled'],
        'title' => $values['general']['title'],
        'name' => $values['general']['name'],
        'blocks' => $blocks,
        'options' => mmenu_convert_settings('options', $values['options']),
        'configurations' => mmenu_convert_settings('configurations', $values['configurations']),
      );
      variable_set('mmenu_item_' . $values['general']['name'], $mmenu);

      // Clears mmenus cache.
      cache_clear_all('mmenus', 'cache');

      drupal_set_message(t('The settings have been saved.'));
      break;

    case t('Reset'):
      // Deletes the mmenu settings from database.
      variable_del('mmenu_item_' . $values['general']['name']);
      // Clears mmenus cache.
      cache_clear_all('mmenus', 'cache');
      drupal_set_message(t('The settings have been reset.'));
      break;
  }
}
