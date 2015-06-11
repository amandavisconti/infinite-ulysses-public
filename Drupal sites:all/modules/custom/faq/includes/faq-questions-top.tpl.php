<?php

/**
 * @file
 * Template file for the FAQ page if set to show the questions in a list at the
 * top.
 */

/**
 * Available variables:
 *
 * $questions_list
 *   Pre-formatted list of questions.
 * $questions
 *   An array of questions to use for producing the question list at the top.
 * $answers
 *   An array of answers to use for producing the main body of text.
 *     $answers[$key]['question'] is the question text.
 *     $answers[$key]['body'] is the answer text.
 *     $answers[$key]['links'] represents the node links, e.g. "Read more".
 * $use_teaser
 *   Is true if $answer['body'] is a teaser.
 * $list_style
 *   Represents the style of list, ul for unordered, ol for ordered.
 * $question_label
 *   The question label, intended to be pre-pended to the question text.
 * $answer_label
 *   The answer label, intended to be pre-pended to the answer text.
 * $limit
 *   Represents the number of items.
 */
?>
<a id="faq-top"></a>
<?php print $questions_list ?>
<br />
<?php $key = 0; ?>
<?php while ($key < $limit): ?>
  <?php /* Cycle through all the answers and "more" links. $key will represent the applicable position in the arrays. */ ?>
  <div class="faq-question">
  <?php if (!empty($question_label)): ?>
    <strong class="faq-question-label">
    <?php print $question_label; ?>
    </strong>
  <?php endif; ?>
  <?php print $answers[$key]['question']; ?>
  </div> <!-- Close div: faq-question -->

  <div class="faq-answer">
  <?php if (!empty($answer_label)): ?>
    <strong class="faq-answer-label">
    <?php print $answer_label; ?>
    </strong>
  <?php endif; ?>
  <?php print $answers[$key]['body']; ?>
  <?php print $answers[$key]['links']; ?>
  </div> <!-- Close div: faq-answer -->
  <?php /* Increment $key to move on to the next position. */ ?>
  <?php $key++; ?>
<?php endwhile; ?>
