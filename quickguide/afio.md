<p>Short text about PFIC</p>

<p>
<details>
<summary>Base addresses</summary>

{{python quickguide-generator/main.py baseaddress AFIO*}}

</details>

<details open>
<summary>
Register Block Overview
</summary>

{{python quickguide-generator/main.py overview-table AFIO}}

</details>

<p>
<hr>

{{python quickguide-generator/main.py register-details AFIO .*}}

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
