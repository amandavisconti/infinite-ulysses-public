
ABOUT
--------------------------------------------------------------------------------
The mobile sliding menu module integrates the mmenu jQuery plugin
for creating slick, app look-alike sliding menus for you mobile website.

REQUIREMENTS
--------------------------------------------------------------------------------
- Libraries module.
- jQuery 1.7.0 or later.

FEATURES
--------------------------------------------------------------------------------
The mmenu jQuery plugin has following features:
- Fully responsive CSS framework generated with SCSS.
- Creates sliding panels as easy as menus.
- Menu can be positioned at the top, right, bottom or left, at the back,
  front or next to the page.
- Use sliding horizontal or expanding vertical submenus.
- Optionally open the menu by dragging the page out of the viewport.
- Plays nicely with jQuery Mobile.
- Add headers, labels, counters and even a search field.
- Completely themable by changing the background-color.
- Works well on all major browsers.
- Filled with options for customizing the menu.
- Uses SCSS to easily create customized menus.

INSTALLATION
--------------------------------------------------------------------------------
1 - First, installs the mmenu module (standard Drupal way).

2 - Adds plugin library.

  2.1.1 - Downloads Mmenu library version 4.5 or above at
          http://mmenu.frebsite.nl/download.php.
  2.1.2 - Unzips the library files in the Drupal path
          "/sites/all/libraries/mmenu/main".
  2.1.3 - For example, the css file could be accessed in
          "/sites/all/libraries/mmenu/main/src/css/jquery.mmenu.all.css".

  2.2.1 - Downloads Hammer.js library version 2.0 or above at
          https://github.com/hammerjs/hammer.js.
  2.2.2 - Unzips the library files in the Drupal path
          "/sites/all/libraries/mmenu/hammer".
  2.2.3 - For example, the js file could be accessed in
          "/sites/all/libraries/mmenu/hammer/hammer.min.js".

  2.3.1 - Downloads jQuery plugin for the Hammer.js library version 2.0 or above
          at https://github.com/EightMedia/jquery.hammer.js.
  2.3.2 - Unzips the library files in the Drupal path
          "/sites/all/libraries/mmenu/jquery.hammer".
  2.3.3 - For example, the js file could be accessed in
          "/sites/all/libraries/mmenu/jquery.hammer/jquery.hammer.js".

  2.4.1 - Since mmenu v2.0, the icomoon data has been removed from the module.
          But you can generate your own icons via https://icomoon.io/app,
          or you can just download it from here
          https://www.drupal.org/files/mmenu-icomoon.tar_.gz.
  2.4.2 - Unzips the icomoon data in the Drupal path
          "/sites/all/libraries/mmenu/icomoon".
  2.4.3 - Copies the styles.css and renames to icomoon.css.
  2.4.4 - For example, the css file could be accessd in
          "/sites/all/libraries/mmenu/icomoon/icomoon.css".

3 - Configurations

  3.1 - Sets the version to 1.7 or above at
        /admin/config/development/jquery_update.

  3.2 - Configs each mmenu at /admin/config/mmenu.
