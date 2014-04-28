<?php

class os_boxes_rand_widget extends os_boxes_widget_collection {

  public $embed_as = array(
    'iframe',
  );

  public function render() {
    $block = parent::render();

    $k = array_rand($this->options['widgets']);
    list(,$delta) = explode('-', $this->options['widgets'][$k]['bid'], 2);
    $box = boxes_box_load($delta);
    if ($box instanceof boxes_box) {
      return $box->render();
    }

    return $block;
  }
}