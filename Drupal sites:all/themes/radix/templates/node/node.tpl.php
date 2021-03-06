<?php

/**
 * @file
 * Radix theme implementation to display a node.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>

  <?php print $user_picture; ?>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
<?php print $title_attributes; ?><a href="<?php print $node_url; ?>"><?php print $title; ?></a>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

    <div class="author">
  	    <?php print $name; ?> 
    </div>


  <div class="content"<?php print $content_attributes; ?>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <div class="metadata">
  <?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>
  <div class="submitted">
    <?php print $submitted; ?>
  </div></article>
</div>