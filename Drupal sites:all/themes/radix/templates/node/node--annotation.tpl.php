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

  <div class="content"<?php print $content_attributes; ?>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
    <div class="author"><?php print $name; ?> </div>
  </div>

<?php $collapsed_content = $submitted."<br><a href=".$node_url.">permalink</a>"
?>

<?php
print theme(
  'ctools_collapsible',
  array(
    'handle' => 'More', 
    'content' => $collapsed_content, 
    'collapsed' => TRUE
  )
);
?>
  
</article>
</div>