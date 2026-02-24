<p>The PFIC (Peripheral Function Interface Controller) is responsible for managing interrupts and events for the microcontroller. It provides a centralized interface for handling various peripheral interrupts.</p>

<p>
<details open>
<summary>Base addresses</summary>

{{python quickguide-generator/main.py baseaddress PFIC*}}

</details>

<details open>
<summary>
Register Block Overview
</summary>

{{python quickguide-generator/main.py overview-table PFIC}}

</details>

<p>
<hr>

{{python quickguide-generator/main.py register-details PFIC .*}}

</p>


<script>
document.addEventListener("DOMContentLoaded", () => {
  document.querySelectorAll(".fold-table .summary").forEach(summary => {
    summary.addEventListener("click", () => {
      let row = summary.nextElementSibling;

      while (row && !row.classList.contains("summary")) {
        row.classList.toggle("open");
        row = row.nextElementSibling;
      }
    });
  });
});
</script>
